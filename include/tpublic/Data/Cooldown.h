#pragma once

#include "../DataBase.h"

namespace tpublic
{

	namespace Data
	{

		struct Cooldown
			: public DataBase
		{
			static const DataType::Id DATA_TYPE = DataType::ID_COOLDOWN;
			static const bool TAGGED = true;

			enum Trigger : uint8_t
			{
				INVALID_TRIGGER,

				TRIGGER_ABILITY,
				TRIGGER_INTERRUPT
			};

			static Trigger
			SourceToTrigger(
				const SourceNode*		aSource) 
			{
				std::string_view t(aSource->GetIdentifier());
				if(t == "ability")
					return TRIGGER_ABILITY;
				else if(t == "interrupt")
					return TRIGGER_INTERRUPT;
				TP_VERIFY(false, aSource->m_debugInfo, "'%s' is not a valid trigger.", aSource->GetIdentifier());
				return INVALID_TRIGGER;
			}

			void
			Verify() const
			{
				VerifyBase();
			}

			// Base implementation
			void
			FromSource(
				const SourceNode*		aNode) override
			{	
				aNode->ForEachChild([&](
					const SourceNode* aChild)
				{
					if(!FromSourceBase(aChild))
					{
						if(aChild->m_name == "duration")
							m_duration = aChild->GetInt32();
						else if(aChild->m_name == "trigger")
							m_trigger = SourceToTrigger(aChild);
						else
							TP_VERIFY(false, aChild->m_debugInfo, "'%s' is not a valid item.", aChild->m_name.c_str());
					}
				});
				
			}

			void
			ToStream(
				IWriter*				aStream) const override
			{
				aStream->WritePOD(m_trigger);
				aStream->WriteInt(m_duration);
			}

			bool
			FromStream(
				IReader*				aStream) override
			{
				if(!aStream->ReadPOD(m_trigger))
					return false;
				if(!aStream->ReadInt(m_duration))
					return false;
				return true;
			}

			// Public data
			Trigger			m_trigger = TRIGGER_ABILITY;
			int32_t			m_duration = 0;
		};

	}

}