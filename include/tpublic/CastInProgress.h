#pragma once

#include "IReader.h"
#include "IWriter.h"

namespace tpublic
{

	class CastInProgress
	{
	public:
		void
		ToStream(
			IWriter*	aWriter) const
		{
			aWriter->WriteUInt(m_abilityId);
			aWriter->WriteUInt(m_targetEntityInstanceId);
			aWriter->WriteInt(m_start);
			aWriter->WriteInt(m_end);
		}

		bool
		FromStream(
			IReader*	aReader) 
		{
			if (!aReader->ReadUInt(m_abilityId))
				return false;
			if (!aReader->ReadUInt(m_targetEntityInstanceId))
				return false;
			if (!aReader->ReadInt(m_start))
				return false;
			if (!aReader->ReadInt(m_end))
				return false;
			return true;
		}

		// Public data
		uint32_t		m_abilityId = 0;
		uint32_t		m_targetEntityInstanceId = 0;
		int32_t			m_start = 0;
		int32_t			m_end = 0;
	};

}