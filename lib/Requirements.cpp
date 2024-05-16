#include "Pcheader.h"

#include <tpublic/Components/ActiveQuests.h>
#include <tpublic/Components/CombatPublic.h>
#include <tpublic/Components/CompletedQuests.h>
#include <tpublic/Components/EquippedItems.h>
#include <tpublic/Components/Inventory.h>
#include <tpublic/Components/Openable.h>
#include <tpublic/Components/PlayerPrivate.h>
#include <tpublic/Components/Position.h>
#include <tpublic/Components/VisibleAuras.h>

#include <tpublic/Data/Entity.h>

#include <tpublic/EntityInstance.h>
#include <tpublic/Manifest.h>

namespace tpublic
{

	namespace Requirements
	{

		bool	
		Check(
			const Manifest*						aManifest,
			const EntityInstance*				aSelf,
			const EntityInstance*				aTarget,
			const Requirement*					aRequirement)
		{
			const EntityInstance* entity = NULL;
			switch (aRequirement->m_target)
			{
			case Requirement::TARGET_SELF:	entity = aSelf; break;
			case Requirement::TARGET_TARGET: entity = aTarget; break;
			default: assert(false); break;
			}

			if (entity == NULL)
				return false;

			switch(aRequirement->m_type)
			{
			case Requirement::TYPE_MUST_HAVE_AURA:
			case Requirement::TYPE_MUST_NOT_HAVE_AURA:
				{
					const Components::VisibleAuras* visibleAuras = entity->GetComponent<Components::VisibleAuras>();
					bool hasAura = visibleAuras->HasAura(aRequirement->m_id);
					if (aRequirement->m_type == Requirement::TYPE_MUST_HAVE_AURA && !hasAura)
						return false;
					else if (aRequirement->m_type == Requirement::TYPE_MUST_NOT_HAVE_AURA && hasAura)
						return false;
				}
				break;					

			case Requirement::TYPE_MUST_HAVE_COMPLETED_QUEST:
			case Requirement::TYPE_MUST_NOT_HAVE_COMPLETED_QUEST:
				{
					if(aRequirement->m_target != Requirement::TARGET_SELF)
						return false;

					const Components::CompletedQuests* completedQuests = entity->GetComponent<Components::CompletedQuests>();
					bool hasCompletedQuest = completedQuests->m_questIds.HasValue(aRequirement->m_id);
					if(aRequirement->m_type == Requirement::TYPE_MUST_HAVE_COMPLETED_QUEST && !hasCompletedQuest)
						return false;
					else if (aRequirement->m_type == Requirement::TYPE_MUST_NOT_HAVE_COMPLETED_QUEST && hasCompletedQuest)
						return false;
				}
				break;

			case Requirement::TYPE_MUST_HAVE_ACTIVE_QUEST:
			case Requirement::TYPE_MUST_NOT_HAVE_ACTIVE_QUEST:
				{
					if(aRequirement->m_target != Requirement::TARGET_SELF)
						return false;

					const Components::ActiveQuests* activeQuests = entity->GetComponent<Components::ActiveQuests>();
					bool hasActiveQuest = activeQuests->HasQuest(aRequirement->m_id);
					if(aRequirement->m_type == Requirement::TYPE_MUST_HAVE_ACTIVE_QUEST && !hasActiveQuest)
						return false;
					else if (aRequirement->m_type == Requirement::TYPE_MUST_NOT_HAVE_ACTIVE_QUEST && hasActiveQuest)
						return false;
				}
				break;

			case Requirement::TYPE_MUST_HAVE_TAG:
				{
					if(entity->IsPlayer())
						return false;

					const Data::Entity* entityData = aManifest->GetById<Data::Entity>(entity->GetEntityId());
					if(!entityData->HasTag(aRequirement->m_id))
						return false;
				}
				break;

			case Requirement::TYPE_MUST_BE_TYPE:
				{
					if(entity->GetEntityId() != aRequirement->m_id)
						return false;
				}
				break;

			case Requirement::TYPE_MUST_BE_IN_STATE:
				{
					if(entity->GetState() != (EntityState::Id)aRequirement->m_id)
						return false;
				}
				break;

			case Requirement::TYPE_MUST_NOT_BE_IN_STATE:
				{
					if(entity->GetState() == (EntityState::Id)aRequirement->m_id)
						return false;
				}
				break;

			case Requirement::TYPE_MUST_BE_HOSTILE:
				{
					const Components::CombatPublic* combatPublic = entity->GetComponent<Components::CombatPublic>();
					const Components::CombatPublic* selfCombatPublic = aSelf->GetComponent<Components::CombatPublic>();
					if(combatPublic->m_factionId == selfCombatPublic->m_factionId)				
						return false;	
				}
				break;

			case Requirement::TYPE_MUST_BE_FACTION:
				{
					const Components::CombatPublic* combatPublic = entity->GetComponent<Components::CombatPublic>();
					if(combatPublic->m_factionId != aRequirement->m_id)
						return false;	
				}
				break;

			case Requirement::TYPE_MUST_BE_GROUP_MEMBER:
				{
					if(entity->GetEntityInstanceId() != aSelf->GetEntityInstanceId())
					{
						const Components::CombatPublic* combatPublic = entity->GetComponent<Components::CombatPublic>();
						if(combatPublic->m_combatGroupId == 0)
							return false;

						const Components::CombatPublic* selfCombatPublic = aSelf->GetComponent<Components::CombatPublic>();
						if (combatPublic->m_combatGroupId != selfCombatPublic->m_combatGroupId)
							return false;
					}
				}
				break;

			case Requirement::TYPE_MUST_HAVE_ITEM_EQUIPPED:
				{
					if (!entity->IsPlayer())
						return false;

					const Components::EquippedItems* equippedItems = entity->GetComponent<Components::EquippedItems>();
					const Data::Item* item = aManifest->GetById<Data::Item>(aRequirement->m_id);
					bool isEquipped = false;

					for(uint32_t equipmentSlotId : item->m_equipmentSlots)
					{
						if(equippedItems->m_slots.m_items[equipmentSlotId].m_itemId == aRequirement->m_id)
						{
							isEquipped = true;
							break;
						}
					}

					if(!isEquipped)
						return false;
				}
				break;

			case Requirement::TYPE_MUST_HAVE_ITEM:
				{
					if (!entity->IsPlayer())
						return false;

					const Components::EquippedItems* equippedItems = entity->GetComponent<Components::EquippedItems>();
					const Data::Item* item = aManifest->GetById<Data::Item>(aRequirement->m_id);
					bool hasItem = false;

					for(uint32_t equipmentSlotId : item->m_equipmentSlots)
					{
						if(equippedItems->m_slots.m_items[equipmentSlotId].m_itemId == aRequirement->m_id)
						{
							hasItem = true;
							break;
						}
					}

					if(!hasItem)
					{
						const Components::Inventory* inventory = entity->GetComponent<Components::Inventory>();
						hasItem = inventory->m_itemList.HasItems(aRequirement->m_id, 1);
					}

					if(!hasItem)
						return false;
				}
				break;

			default:
				assert(false);
				break;
			}

			return true;
		}

		bool	
		CheckList(
			const Manifest*										aManifest,
			const std::vector<Requirement>&						aRequirements,
			const EntityInstance*								aSelf,
			const EntityInstance*								aTarget,
			const Requirement**									aOutFailedRequirement)
		{
			for(const Requirement& requirement : aRequirements)
			{
				if(!Check(aManifest, aSelf, aTarget, &requirement))
				{
					if(aOutFailedRequirement != NULL)
						*aOutFailedRequirement = &requirement;
					return false;
				}
			}

			return true;
		}

		bool	
		CheckAnyList(
			const Manifest*										aManifest,
			const std::vector<Requirement>&						aRequirements,
			const std::vector<const EntityInstance*>&			aSelves,
			const EntityInstance*								aTarget)
		{
			for(const EntityInstance* self : aSelves)
			{
				bool ok = true;

				for (const Requirement& requirement : aRequirements)
				{
					if (!Check(aManifest, self, aTarget, &requirement))
					{
						ok = false;
						break;
					}
				}

				if(ok)
					return true;
			}
			return false;
		}

		bool	
		CheckOpenable(
			const EntityInstance*								aSelf,
			const EntityInstance*								aTarget,
			bool*												aOutInstant)
		{
			const Components::Openable* openable = aTarget->GetComponent<Components::Openable>();
			if(openable == NULL)
				return false;

			const Components::Position* selfPosition = aSelf->GetComponent<Components::Position>();
			const Components::Position* targetPosition = aTarget->GetComponent<Components::Position>();
			if(selfPosition->m_position.DistanceSquared(targetPosition->m_position) > (int32_t)(openable->m_range * openable->m_range))
				return false;
			
			if (openable->m_requiredProfessionId != 0)
			{
				const tpublic::Components::PlayerPrivate* playerPrivate = aSelf->GetComponent<tpublic::Components::PlayerPrivate>();
				if(!playerPrivate->m_professions.HasProfessionSkill(openable->m_requiredProfessionId, openable->m_requiredProfessionSkill))
					return false;
			}

			if (openable->m_requiredItemId != 0)
			{
				const tpublic::Components::Inventory* inventory = aSelf->GetComponent<tpublic::Components::Inventory>();
				if(!inventory->m_itemList.HasItems(openable->m_requiredItemId, 1))
				{
					const tpublic::Components::EquippedItems* equippedItems = aSelf->GetComponent<tpublic::Components::EquippedItems>();
					bool hasItem = false;
					equippedItems->ForEach([&](
						const tpublic::ItemInstance& aItemInstance)
					{
						if(aItemInstance.m_itemId == openable->m_requiredItemId)
							hasItem = true;
					});
					
					if(!hasItem)
						return false;
				}
			}

			if(openable->m_requiredQuestId)
			{
				const tpublic::Components::ActiveQuests* activeQuests = aSelf->GetComponent<tpublic::Components::ActiveQuests>();
				if(!activeQuests->HasQuest(openable->m_requiredQuestId))
					return false;
			}

			if(aOutInstant != NULL)
				*aOutInstant = openable->m_instant;

			return true;
		}

	}

}