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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @fn AC_ERROR AC_API acChunkDataGetChunk(acBuffer hBuffer, const char* pName, acNode* phChunk)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A chunk data
 *
 * @param pName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the chunk
 *  - Prefixed with 'Chunk'
 *
 * @param phChunk
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Requested node
 *  - Null on failure
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acChunkDataGetChunk </B> gets a specified chunk, which can be used to
 * retrieve the chunk value and related information available in its node type
 * (maximum, minimum, node display name, etc).
 */
AC_ERROR AC_API acChunkDataGetChunk(acBuffer hBuffer, const char* pName, acNode* phChunk);

/**
 * @fn AC_ERROR AC_API acChunkDataGetChunkAndAccessMode(acBuffer hBuffer, const char* pName, acNode* phChunkNode, AC_ACCESS_MODE* pAccessMode)
 *
 * @param hBuffer
 *  - Type: acBuffer
 *  - [In] parameter
 *  - A chunk data
 *
 * @param pName
 *  - Type: const char*
 *  - [In] parameter
 *  - Name of the chunk
 *  - Prefixed with 'Chunk'
 *
 * @param phChunkNode
 *  - Type: acNode*
 *  - [Out] parameter
 *  - Requested node
 *  - Null on failure
 *
 * @param pAccessMode
 *  - Type: AC_ACCESS_MODE*
 *  - [Out] parameter
 *  - Access mode of the requested node
 *
 * @return 
 *  - Type: AC_ERROR
 *  - Error code for the function
 *  - Returns AC_ERR_SUCCESS (0) on success
 *
 * <B> acChunkDataGetChunkAndAccessMode </B> gets a specified chunk and its
 * access mode, returning it as a node in order to preserve metadata related to
 * the chunk.
 */
AC_ERROR AC_API acChunkDataGetChunkAndAccessMode(acBuffer hBuffer, const char* pName, acNode* phChunkNode, AC_ACCESS_MODE* pAccessMode);

#ifdef __cplusplus
} // extern "C"
#endif
