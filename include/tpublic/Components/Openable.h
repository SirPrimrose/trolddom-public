#pragma once

#include "../ComponentBase.h"

namespace tpublic
{

	namespace Components
	{

		struct Openable
			: public ComponentBase
		{
			static const Component::Id ID = Component::ID_OPENABLE;
			static const uint8_t FLAGS = 0;
			static const Persistence::Id PERSISTENCE = Persistence::ID_NONE;
			static const Replication REPLICATION = REPLICATION_PUBLIC;

			struct Verb
			{
				enum Type : uint8_t
				{
					TYPE_OPEN,
					TYPE_PICK,
					TYPE_MINE
				};

				void
				FromSource(
					const SourceNode*	aSource)
				{
					std::string_view t(aSource->GetIdentifier());
					if (t == "open")
						m_type = TYPE_OPEN;
					else if (t == "pick")
						m_type = TYPE_PICK;
					else if (t == "mine")
						m_type = TYPE_MINE;
					else
						TP_VERIFY(false, aSource->m_debugInfo, "'%s' is not a valid verb.", aSource->GetIdentifier());
				}

				void
				ToStream(
					IWriter*			aWriter) const
				{
					aWriter->WritePOD(m_type);
				}

				bool
				FromStream(
					IReader*			aReader) 
				{
					if(!aReader->ReadPOD(m_type))
						return false;
					return true;
				}

				const char*
				GetDisplayString() const
				{
					switch(m_type)
					{
					case TYPE_OPEN:	return "Open";
					case TYPE_PICK:	return "Pick";
					case TYPE_MINE:	return "Mine";
					default:		break;
					}
					assert(false);
					return "";
				}

				// Public data
				Type				m_type = TYPE_OPEN;
			};

			enum Field
			{
				FIELD_LOOT_TABLE_ID,
				FIELD_REQUIRED_ITEM_ID,
				FIELD_VERB,
				FIELD_REQUIRED_PROFESSION_ID,
				FIELD_REQUIRED_PROFESSION_SKILL
			};

			static void
			CreateSchema(
				ComponentSchema*		aSchema)
			{
				aSchema->Define(ComponentSchema::TYPE_UINT32, FIELD_LOOT_TABLE_ID, "loot_table", offsetof(Openable, m_lootTableId))->SetDataType(DataType::ID_LOOT_TABLE);
				aSchema->Define(ComponentSchema::TYPE_UINT32, FIELD_REQUIRED_ITEM_ID, "required_item", offsetof(Openable, m_requiredItemId))->SetDataType(DataType::ID_ITEM);
				aSchema->DefineCustomObject<Verb>(FIELD_VERB, "verb", offsetof(Openable, m_verb));
				aSchema->Define(ComponentSchema::TYPE_UINT32, FIELD_REQUIRED_PROFESSION_ID, "required_profession", offsetof(Openable, m_requiredProfessionId))->SetDataType(DataType::ID_PROFESSION);
				aSchema->Define(ComponentSchema::TYPE_UINT32, FIELD_REQUIRED_PROFESSION_SKILL, "required_profession_skill", offsetof(Openable, m_requiredProfessionSkill));
			}

			// Public data
			uint32_t				m_lootTableId = 0;
			uint32_t				m_requiredItemId = 0;
			Verb					m_verb;
			uint32_t				m_requiredProfessionId = 0;
			uint32_t				m_requiredProfessionSkill = 0;
		};

	}

}