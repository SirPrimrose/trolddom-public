#pragma once

#include "../Components/Combat.h"

#include "../EffectBase.h"
#include "../EntityInstance.h"

namespace kaos_public
{

	namespace Effects
	{

		struct Kill
			: public EffectBase
		{
			static const Effect::Id ID = Effect::ID_KILL;

			Kill()
			{

			}

			virtual 
			~Kill()
			{

			}

			// EffectBase implementation
			void
			FromSource(
				const Parser::Node*		/*aSource*/) override
			{
			}

			void
			ToStream(
				IWriter*				aStream) const override
			{
				ToStreamBase(aStream);
			}

			bool
			FromStream(
				IReader*				aStream) override
			{
				if(!FromStreamBase(aStream))
					return false;
				return true;
			}

			void	
			Apply(
				EntityInstance*			aEntity,
				uint32_t				aTick) override
			{
				aEntity->SetState(EntityState::ID_DEAD, aTick);
			}

			// Public data
		};

	}

}