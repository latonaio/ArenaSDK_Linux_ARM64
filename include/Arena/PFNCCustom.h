/***************************************************************************************
 ***                                                                                 ***
 ***  Copyright (c) 2019, Lucid Vision Labs, Inc.                                    ***
 ***                                                                                 ***
 ***  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     ***
 ***  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       ***
 ***  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    ***
 ***  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         ***
 ***  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ***
 ***  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  ***
 ***  SOFTWARE.                                                                      ***
 ***                                                                                 ***
 ***************************************************************************************/
#pragma once

namespace Arena
{
	/**
	 * @fn size_t GetBitsPerPixel(uint64_t pixelFormat)
	 *
	 * @param pixelFormat
	 *  - Type: uint64_t
	 *  - Represents: PfncFormat
	 *  - Pixel format to get bits per pixel
	 *
	 * @return 
	 *  - Type: size_t
	 *  - Number of bits per pixel for given pixel format
	 *
	 * <B> GetBitsPerPixel </B> get the number of bits per pixel of the image
	 * from the integer value of the pixel format (PfncFormat).
	 *
	 * Pixel format values are determined by the PFNC (Pixel Format Naming
	 * Convention) specification. The PFNC assigns a name and number to each
	 * pixel format helping to standardize pixel formats. The number of bits per
	 * pixel can be found in each integer at bytes 5 and 6 (mask 0x00FF0000). The
	 * pixel format can be determined by the integer using the GetPixelFormatName
	 * function provided by the PFNC.
	 */
	ARENA_API size_t GetBitsPerPixel(uint64_t pixelFormat);
}
