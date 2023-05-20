#include "Pcheader.h"

#include <brotli/decode.h>
#include <brotli/encode.h>

#include <tpublic/Compression.h>

namespace tpublic
{

	namespace Compression
	{

		void		
		Pack(
			const void*				aBuffer,
			size_t					aBufferSize,
			Level					aLevel,
			std::vector<uint8_t>&	aOut)
		{	
			size_t maxEncodedSize = BrotliEncoderMaxCompressedSize(aBufferSize);
			aOut.resize(maxEncodedSize);

			size_t encodedSize;

			int quality = 0;
			int windowBits = 0;

			if(aLevel == LEVEL_BEST)
			{
				quality = BROTLI_MAX_QUALITY;
				windowBits = BROTLI_MAX_WINDOW_BITS;
			}
			else if (aLevel == LEVEL_FAST)
			{
				quality = BROTLI_MIN_QUALITY;
				windowBits = BROTLI_MIN_WINDOW_BITS;
			}
			else
			{
				assert(false);
			}

			BROTLI_BOOL result = BrotliEncoderCompress(
				quality, 
				windowBits, 
				BROTLI_MODE_GENERIC,
				aBufferSize,
				(const uint8_t*)aBuffer,
				&encodedSize,
				&aOut[0]);
			assert(result != 0);

			aOut.resize(encodedSize + sizeof(size_t));

			size_t* size = (size_t*)&aOut[aOut.size() - sizeof(size_t)];
			*size = aBufferSize;
		}
		
		bool
		Unpack(
			const void*				aBuffer,
			size_t					aBufferSize,
			std::vector<uint8_t>&	aOut)
		{
			if(aBufferSize < sizeof(size_t))
				return false;

			size_t uncompressedSize = *((size_t*)(((const uint8_t*)aBuffer) + aBufferSize - sizeof(size_t)));
			size_t maxEncodedSize = BrotliEncoderMaxCompressedSize(uncompressedSize);
			if(aBufferSize - sizeof(size_t) > maxEncodedSize)
				return false;

			aOut.resize(uncompressedSize);

			size_t decodedSize;

			BROTLI_BOOL result = BrotliDecoderDecompress(
				aBufferSize - sizeof(size_t),
				(const uint8_t*)aBuffer,
				&decodedSize,
				&aOut[0]);
			if(result == 0)
				return false;

			if(decodedSize != uncompressedSize)
				return false;

			return true;
		}

	}

}