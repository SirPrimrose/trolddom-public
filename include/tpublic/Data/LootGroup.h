#pragma once

#include "../DataBase.h"

namespace tpublic
{

	namespace Data
	{

		struct LootGroup
			: public DataBase
		{
			static const DataType::Id DATA_TYPE = DataType::ID_LOOT_GROUP;
			static const bool TAGGED = true;

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
						TP_VERIFY(false, aChild->m_debugInfo, "'%s' is not a valid item.", aChild->m_name.c_str());
				});
			}

			void
			ToStream(
				IWriter*				/*aStream*/) const override
			{
			}

			bool
			FromStream(
				IReader*				/*aStream*/) override
			{
				return true;
			}

			// Public data
		};

	}

}