/*
 * Copyright 2012, 2013 Moritz Hilscher
 *
 * This file is part of mapcrafter.
 *
 * mapcrafter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mapcrafter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mapcrafter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#include "nbt.h"
#include "pos.h"

#include <stdint.h>

namespace mapcrafter {
namespace mc {

/**
 * A 16x16x16 section of a chunk.
 */
struct ChunkSection {
	uint8_t y;
	uint8_t blocks[16 * 16 * 16];
	uint8_t add[16 * 16 * 8];
	uint8_t data[16 * 16 * 8];
	uint8_t block_light[16 * 16 * 8];
	uint8_t sky_light[16 * 16 * 8];

	const uint8_t* getArray(int i) const {
		if (i == 0)
			return data;
		else if (i == 1)
			return block_light;
		else
			return sky_light;
	}
};

/**
 * This is a Minecraft chunk from the Anvil world format. The class stores only the
 * sections, which exist in the nbt data, to save memory.
 */
class Chunk {
private:
	ChunkPos pos;

	int rotation;

	// the index of the chunk sections in the sections array
	// or -1 if section does not exists
	int section_offsets[16];
	// the array with the sections, see indexes above
	std::vector<ChunkSection> sections;

	uint8_t biomes[256];

	uint8_t getData(const LocalBlockPos& pos, int array) const;
public:
	Chunk();
	virtual ~Chunk();

	void setRotation(int rotation);

	bool readNBT(const char* data, size_t len, nbt::Compression compression =
	        nbt::Compression::ZLIB);
	void clear();

	bool hasSection(int section) const;
	uint16_t getBlockID(const LocalBlockPos& pos) const;
	uint8_t getBlockData(const LocalBlockPos& pos) const;

	uint8_t getBlockLight(const LocalBlockPos& pos) const;
	uint8_t getSkyLight(const LocalBlockPos& pos) const;

	uint8_t getBiomeAt(const LocalBlockPos& pos) const;

	const ChunkPos& getPos() const;
};

}
}

#endif /* CHUNK_H_ */
