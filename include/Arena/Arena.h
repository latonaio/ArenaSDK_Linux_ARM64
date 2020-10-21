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

#include "ISystem.h"

namespace Arena
{
	/**
	 * @fn ISystem* OpenSystem()
	 *
	 * @return 
	 *  - Type: ISystem*
	 *  - The system object
	 *
	 * <B> OpenSystem </B> initializes the Arena SDK and retrieves the system
	 * object (Arena::ISystem). The system must be closed or memory will leak.
	 *
	 * \code{.cpp}
	 * 	// opening and closing a system
	 * 	{
	 * 		Arena::ISystem* pSystem = Arena::OpenSystem();
	 * 		// ...
	 * 		Arena::CloseSystem(pSystem);
	 * 	}
	 * \endcode
	 *
	 * @warning 
	 *  - Only one system may be opened at a time
	 *  - System must be closed
	 *  - May throw GenICam::GenericException or other derived exception
	 *
	 * @see 
	 *  - Arena::ISystem
	 *  - Arena::OpenSystem
	 *  - Arena::CloseSystem
	 */
	ARENA_API ISystem* OpenSystem();

	/**
	 * @fn void CloseSystem(ISystem* pSystem)
	 *
	 * @param pSystem
	 *  - Type: ISystem*
	 *  - The system object
	 *
	 * @return 
	 *  - none
	 *
	 * <B> CloseSystem </B> cleans up the system (Arena::ISystem) and
	 * deinitializes the Arena SDK, deallocating all memory.
	 *
	 * @warning 
	 *  - The system must be closed
	 *  - May throw GenICam::GenericException or other derived exception
	 *
	 * @see 
	 *  - Arena::ISystem
	 */
	ARENA_API void CloseSystem(ISystem* pSystem);

	/**
	 * uint64_t CalculateMaximumNumberOfBuffers(size_t payloadSize)
	 *
	 * @param payloadSize
	 *	- Type: size_t
	 *	- Unit: bytes
	 *	- Payload size of an image
	 *
	 * @return
	 *	- Type: uint64_t
	 *	- Maximum number of buffers
	 *
	 * <B> CalculateMaximumNumberOfBuffers </B> calculates the number of buffers
	 * it would take to fill 80% of the available memory.
	 */
	ARENA_API size_t CalculateMaximumNumberOfBuffers(size_t payloadSize);
}
