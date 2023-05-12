#pragma once

namespace tpublic
{

	class SpriteInfo
	{
	public:
		enum Flag : uint8_t
		{
			FLAG_TILE_WALKABLE				= 0x01,
			FLAG_TILE_BLOCK_LINE_OF_SIGHT	= 0x02
		};

		static inline uint8_t 
		StringToFlag(			
			const char*		aString)
		{
			if (strcmp(aString, "tile_walkable") == 0)
				return FLAG_TILE_WALKABLE;
			if (strcmp(aString, "tile_block_line_of_sight") == 0)
				return FLAG_TILE_BLOCK_LINE_OF_SIGHT;

			return 0;
		}

		void
		ToStream(
			IWriter*		aStream) const 
		{
			aStream->WritePOD(m_flags);
			aStream->WriteUInt(m_tileLayer);
			aStream->WriteUInts(m_borders);
			aStream->WriteUInt(m_animationNextFrame);
			aStream->WriteUInt(m_animationDelay);
		}

		bool
		FromStream(
			IReader*		aStream) 
		{
			if (!aStream->ReadPOD(m_flags))
				return false;
			if (!aStream->ReadUInt(m_tileLayer))
				return false;
			if (!aStream->ReadUInts(m_borders))
				return false;
			if (!aStream->ReadUInt(m_animationNextFrame))
				return false;
			if (!aStream->ReadUInt(m_animationDelay))
				return false;
			return true;
		}

		// Public data
		uint8_t					m_flags = 0;
		uint32_t				m_tileLayer = 0;
		std::vector<uint32_t>	m_borders;
		uint32_t				m_animationNextFrame = 0;
		uint32_t				m_animationDelay = 0;
	};

}