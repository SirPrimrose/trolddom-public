#pragma once

namespace tpublic
{

	namespace Components
	{
		struct Lootable;
	}

	class Manifest;

	class LootGenerator
	{
	public:
					LootGenerator(
						const Manifest*				aManifest);			
					~LootGenerator();

		void		Generate(
						std::mt19937&				aRandom,
						uint32_t					aLevel,
						uint32_t					aCreatureTypeId,
						bool						aIsElite,
						Components::Lootable*		aLootable) const;

	private:

		const Manifest*					m_manifest;

		struct LevelBucket
		{
			std::vector<uint32_t>		m_itemIds;
		};

		struct Group
		{
			LevelBucket*
			GetOrCreateLevelBucket(
				uint32_t						aLevel)
			{
				LevelBucketTable::iterator i = m_levelBucketTable.find(aLevel);
				if (i != m_levelBucketTable.end())
					return i->second.get();

				LevelBucket* t = new LevelBucket();
				m_levelBucketTable[aLevel] = std::unique_ptr<LevelBucket>(t);
				return t;
			}

			const LevelBucket*
			GetLevelBucket(
				uint32_t						aLevel) const
			{
				LevelBucketTable::const_iterator i = m_levelBucketTable.find(aLevel);
				if (i == m_levelBucketTable.cend())
					return NULL;
				return i->second.get();
			}

			// Public data	
			typedef std::unordered_map<uint32_t, std::unique_ptr<LevelBucket>> LevelBucketTable;
			LevelBucketTable			m_levelBucketTable;
			LevelBucket					m_defaultLevelBucket;
		};

		typedef std::unordered_map<uint32_t, std::unique_ptr<Group>> GroupTable;
		GroupTable						m_groups;

		Group*		_GetOrCreateGroup(
						uint32_t				aLootGroupId);
	};

}