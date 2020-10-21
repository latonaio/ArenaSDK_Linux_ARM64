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
 * @fn SC_ERROR SC_API saveRecorderCreateEmpty(saveRecorder* hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder*
 *  - [Out] parameter
 *  - Created video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderCreateEmpty </B> creates an empty/default video recorder. The
 * parameters of the video recorder are not yet set (width = height = 0, FPS =
 * 0/0). The default file name pattern is 'savedvideos/video<count>.avi'. The
 * default codec is raw, the default container is AVI, and the default pixel
 * format is BGR8.
 */
SC_ERROR SC_API saveRecorderCreateEmpty(saveRecorder* hRecorder);

/**
 * @fn SC_ERROR SC_API saveRecorderCreate(size_t width, size_t height, double fps, saveRecorder* hRecorder)
 *
 * @param width
 *  - Type: size_t
 *  - [In] parameter
 *  - Width of a single frame in the video
 *
 * @param height
 *  - Type: size_t
 *  - [In] parameter
 *  - Height of a single frame in the video
 *
 * @param fps
 *  - Type: double
 *  - [In] parameter
 *  - Frames per second
 *
 * @param hRecorder
 *  - Type: saveRecorder*
 *  - [Out] parameter
 *  - Created video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderCreate </B> creates a video recorder and sets initial video
 * parameters. The default file name pattern is 'savedvideos/video<count>.avi'.
 * The default codec is raw, the default container is AVI, and the default pixel
 * format is BGR8.
 */
SC_ERROR SC_API saveRecorderCreate(size_t width, size_t height, double fps, saveRecorder* hRecorder);

/**
 * @fn SC_ERROR SC_API saveRecorderDestroy(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderDestroy </B> cleans up a video recorder (saveRecorder),
 * deallocating its memory.
 *
 * @see 
 *  - saveRecorder
 */
SC_ERROR SC_API saveRecorderDestroy(saveRecorder hRecorder);

/**
 * @fn SC_ERROR SC_API saveRecorderSetRaw(saveRecorder hRecorder, uint64_t pixelFormat)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pixelFormat
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Pixel format of the image data
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetRaw </B> changes the image writer to save images using a
 * specified pixel format without a container.
 *
 * @warning 
 *  - changes file extension
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetRaw(saveRecorder hRecorder, uint64_t pixelFormat);

/**
 * @fn SC_ERROR SC_API saveRecorderSetRawAviBGR8(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetRawAviBGR8 </B> sets the codec, container, and pixel format
 * to raw, AVI, and BGR8, respectively.
 *
 * @warning 
 *  - pushing non-BGR8 data will result in unexpected behavior
 *  - changes file extension
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetRawAviBGR8(saveRecorder hRecorder);

/**
 * @fn SC_ERROR SC_API saveRecorderSetRawMovRGB8(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetRawMovRGB8 </B> sets the codec, container, and pixel format
 * to raw, MOV, and RGB8, respectively.
 *
 * @warning 
 *  - pushing non-RGB8 data will result in unexpected behavior
 *  - changes file extension
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetRawMovRGB8(saveRecorder hRecorder);

/**
 * @fn SC_ERROR SC_API saveRecorderSetH264MovRGB8(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetH264MovRGB8 </B> sets the codec, container, and pixel
 * format to H264, MOV, and RGB8, respectively.
 *
 * @warning 
 *  - pushing non-RGB8 data will result in unexpected behavior
 *  - changes file extension
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetH264MovRGB8(saveRecorder hRecorder);

/**
* @fn SC_ERROR SC_API saveRecorderSetH264MovRGB8AndConfig(saveRecorder hRecorder, int64_t bitrate)
*
* @param hRecorder
*  - Type: saveRecorder
*  - [In] parameter
*  - A video recorder
*
* @param bitRate
*  - Type: int64_t
*  - [In] parameter
*  - Bitrate (bit/s)
*
* @return
*  - Type: SC_ERROR
*  - Error code for the function
*  - Returns SC_ERR_SUCCESS (0) on success
*
* <B> saveRecorderSetH264MovRGB8 </B> sets the codec, container, and pixel
* format to H264, MOV, and RGB8, respectively.
*
* @warning
*  - pushing non-RGB8 data will result in unexpected behavior
*  - changes file extension
*  - cannot be called while recording
*/
SC_ERROR SC_API saveRecorderSetH264MovRGB8AndConfig(saveRecorder hRecorder, int64_t bitrate);

/**
 * @fn SC_ERROR SC_API saveRecorderSetH264MovBGR8(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetH264MovBGR8 </B> sets the codec, container, and pixel
 * format to H264, MOV, and BGR8, respectively.
 *
 * @warning 
 *  - pushing non-BGR8 data will result in unexpected behavior
 *  - changes file extension
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetH264MovBGR8(saveRecorder hRecorder);

/**
* @fn SC_ERROR SC_API saveRecorderSetH264MovBGR8AndConfig(saveRecorder hRecorder, int64_t bitrate)
*
* @param hRecorder
*  - Type: saveRecorder
*  - [In] parameter
*  - A video recorder
*
* @param bitRate
*  - Type: int64_t
*  - [In] parameter
*  - Bitrate (bit/s)
*
* @return
*  - Type: SC_ERROR
*  - Error code for the function
*  - Returns SC_ERR_SUCCESS (0) on success
*
* <B> saveRecorderSetH264MovBGR8 </B> sets the codec, container, and pixel
* format to H264, MOV, and BGR8, respectively.
*
* @warning
*  - pushing non-BGR8 data will result in unexpected behavior
*  - changes file extension
*  - cannot be called while recording
*/
SC_ERROR SC_API saveRecorderSetH264MovBGR8AndConfig(saveRecorder hRecorder, int64_t bitrate);

/**
 * @fn SC_ERROR SC_API saveRecorderSetH264Mp4RGB8(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetH264Mp4RGB8 </B> sets the codec, container, and pixel
 * format to H264, MPEG-4, and RGB8, respectively.
 *
 * @warning 
 *  - pushing non-RGB8 data will result in unexpected behavior
 *  - changes file extension
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetH264Mp4RGB8(saveRecorder hRecorder);

/**
* @fn SC_ERROR SC_API saveRecorderSetH264Mp4RGB8AndConfig(saveRecorder hRecorder, int64_t bitrate)
*
* @param hRecorder
*  - Type: saveRecorder
*  - [In] parameter
*  - A video recorder
*
* @param bitRate
*  - Type: int64_t
*  - [In] parameter
*  - Bitrate (bit/s)
*
* @return
*  - Type: SC_ERROR
*  - Error code for the function
*  - Returns SC_ERR_SUCCESS (0) on success
*
* <B> saveRecorderSetH264Mp4RGB8 </B> sets the codec, container, and pixel
* format to H264, MPEG-4, and RGB8, respectively.
*
* @warning
*  - pushing non-RGB8 data will result in unexpected behavior
*  - changes file extension
*  - cannot be called while recording
*/
SC_ERROR SC_API saveRecorderSetH264Mp4RGB8AndConfig(saveRecorder hRecorder, int64_t bitrate);

/**
 * @fn SC_ERROR SC_API saveRecorderSetH264Mp4BGR8(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetH264Mp4BGR8 </B> sets the codec, container, and pixel
 * format to H264, MPEG-4, and BGR8, respectively.
 *
 * @warning 
 *  - pushing non-BGR8 data will result in unexpected behavior
 *  - changes file extension
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetH264Mp4BGR8(saveRecorder hRecorder);

/**
* @fn SC_ERROR SC_API saveRecorderSetH264Mp4BGR8AndConfig(saveRecorder hRecorder, int64_t bitrate)
*
* @param hRecorder
*  - Type: saveRecorder
*  - [In] parameter
*  - A video recorder
*
* @param bitRate
*  - Type: int64_t
*  - [In] parameter
*  - Bitrate (bit/s)
*
* @return
*  - Type: SC_ERROR
*  - Error code for the function
*  - Returns SC_ERR_SUCCESS (0) on success
*
* <B> saveRecorderSetH264Mp4BGR8 </B> sets the codec, container, and pixel
* format to H264, MPEG-4, and BGR8, respectively.
*
* @warning
*  - pushing non-BGR8 data will result in unexpected behavior
*  - changes file extension
*  - cannot be called while recording
*/
SC_ERROR SC_API saveRecorderSetH264Mp4BGR8AndConfig(saveRecorder hRecorder, int64_t bitrate);

/**
 * @fn SC_ERROR SC_API saveRecorderSetParams(saveRecorder hRecorder, size_t width, size_t height, double fps)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param width 
 *  - Type: size_t
 *  - [In] parameter
 *  - Width of a single frame in the video
 *
 * @param height
 *  - Type: size_t
 *  - [In] parameter
 *  - Height of a single frame in the video
 *
 * @param fps
 *  - Type: double
 *  - [In] parameter
 *  - Frames per second
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetParams </B> sets the parameters for the video(s) to
 * recorder.
 *
 * @warning 
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetParams(saveRecorder hRecorder, size_t width, size_t height, double fps);

/**
 * @fn SC_ERROR SC_API saveRecorderSetFileNamePattern(saveRecorder hRecorder, const char* pFileNamePattern)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pFileNamePattern
 *  - Type: const char*
 *  - [In] parameter
 *  - File name pattern to set
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetFileNamePattern </B> sets a file name pattern. Changing the
 * extension via this call will change the output container.
 *
 * The video recorder defaults to different settings depending on the extension
 * of the file name pattern:
 *  - '.avi' defaults to raw codec, BGR8 pixel format
 *  - '.mov' defaults to raw codec, RGB8 pixel format
 *  - '.mp4' defaults to H264 codec, BGR8 pixel
 *
 * @warning 
 *  - changes to the file extension will change the container accordingly
 *  - set file name pattern before setting other settings
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetFileNamePattern(saveRecorder hRecorder, const char* pFileNamePattern);
#ifdef _WIN32
SC_ERROR SC_API saveRecorderSetFileNamePatternU(saveRecorder hRecorder, const wchar_t* pFileNamePattern);
#endif
/**
 * @fn SC_ERROR SC_API saveRecorderUpdateTag(saveRecorder hRecorder, const char* pTag, const char* pValue)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pTag
 *  - Type: const char*
 *  - [In] parameter
 *  - Tag to replace
 *
 * @param pValue
 *  - Type: const char*
 *  - [In] parameter
 *  - Value to set
 *
 * @return 
 *  - none
 *
 * <B> saveRecorderUpdateTag </B> updates the value to replace a given tag when
 * an video is saved.
 */
SC_ERROR SC_API saveRecorderUpdateTag(saveRecorder hRecorder, const char* pTag, const char* pValue);
#ifdef _WIN32
SC_ERROR SC_API saveRecorderUpdateTagU(saveRecorder hRecorder, const wchar_t* pTag, const wchar_t* pValue);
#endif

/**
 * @fn SC_ERROR SC_API saveRecorderSetCount(saveRecorder hRecorder, uint64_t count)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param count
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Value of the internal counter
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetCount </B> manually sets the count. The count increments
 * with each recorded video. The count is context specific.
 *
 * @warning 
 *  - cannot be called while recording
 */
SC_ERROR SC_API saveRecorderSetCount(saveRecorder hRecorder, uint64_t count);

/**
 * @fn SC_ERROR SC_API saveRecorderSetTimestamp(saveRecorder hRecorder, uint64_t timestamp)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param timestamp
 *  - Type: uint64_t
 *  - [In] parameter
 *  - Timestamp to set
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderSetTimestamp </B> manually sets the timestamp. The timestamp
 * must be updated manually.
 *
 * @warning 
 *  - cannot be called while recording
 *
 * @deprecated 
 *  - Deprecated in favor of the more generic tag and value components
 */
SC_ERROR SC_API saveRecorderSetTimestamp(saveRecorder hRecorder, uint64_t timestamp);

/**
 * @fn SC_ERROR SC_API saveRecorderGetParams(saveRecorder hRecorder, size_t* pWidth, size_t* pHeight, double* pFps)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pWidth
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Accepts null
 *  - The width
 *
 * @param pHeight
 *  - Type: size_t*
 *  - [Out] parameter
 *  - Accepts null
 *  - The height
 *
 * @param pFps
 *  - Type: double*
 *  - [Out] parameter
 *  - Accepts null
 *  - Frames per second
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderGetParams </B> retrieves the parameters currently used to
 * recorder videos.
 *
 * @warning 
 *  - At least one [Out] parameter must be non-null
 */
SC_ERROR SC_API saveRecorderGetParams(saveRecorder hRecorder, size_t* pWidth, size_t* pHeight, double* pFps);

/**
 * @fn SC_ERROR SC_API saveRecorderGetFileNamePattern(saveRecorder hRecorder, char* pFileNamePattern, size_t* pLen)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pFileNamePattern
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - File name pattern
 *
 * @param pLen
 *  - Type: size_t*
 *  - [In/Out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value - including the null terminating character -
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderGetFileNamePattern </B> retrieves the file name pattern. The
 * received file name pattern does not replace tags (e.g. <count>, <timestamp>).
 */
SC_ERROR SC_API saveRecorderGetFileNamePattern(saveRecorder hRecorder, char* pFileNamePattern, size_t* pLen);
#ifdef _WIN32
SC_ERROR SC_API saveRecorderGetFileNamePatternU(saveRecorder hRecorder, wchar_t* pFileNamePattern, size_t* pLen);
#endif
/**
 * @fn SC_ERROR SC_API saveRecorderGetExtension(saveRecorder hRecorder, char* pExtension, size_t* pLen)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pExtension
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Current extension
 *
 * @param pLen
 *  - Type: size_t*
 *  - [In/Out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value - including the null terminating character -
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderGetExtension </B> retrieves the extension currently being used
 * (e.g. '.avi', '.mp4').
 */
SC_ERROR SC_API saveRecorderGetExtension(saveRecorder hRecorder, char* pExtension, size_t* pLen);
#ifdef _WIN32
SC_ERROR SC_API saveRecorderGetExtensionU(saveRecorder hRecorder, wchar_t* pExtension, size_t* pLen);
#endif

/**
 * @fn SC_ERROR SC_API saveRecorderPeekFileName(saveRecorder hRecorder, char* pFileName, size_t* pLen)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pFileName
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Next file name to use
 *
 * @param pLen
 *  - Type: size_t*
 *  - [In/Out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value - including the null terminating character -
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderPeekFileName </B> peeks at the next file name to use,
 * replacing any appropriate tags (e.g. <count>, <timestamp>).
 */
SC_ERROR SC_API saveRecorderPeekFileName(saveRecorder hRecorder, char* pFileName, size_t* pLen);
#ifdef _WIN32
SC_ERROR SC_API saveRecorderPeekFileNameU(saveRecorder hRecorder, wchar_t* pFileName, size_t* pLen);
#endif

/**
 * @fn SC_ERROR SC_API saveRecorderPeekCount(saveRecorder hRecorder, uint64_t* pCount)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pCount
 *  - Type: uint64_t*
 *  - [Out] parameter
 *  - Local count
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderPeekCount </B> peeks at the count/index of the next saved
 * image. This is the number that the <count> tag will be replaced with.
 */
SC_ERROR SC_API saveRecorderPeekCount(saveRecorder hRecorder, uint64_t* pCount);

/**
 * @fn SC_ERROR SC_API saveRecorderPeekCountPath(saveRecorder hWriter, uint64_t* pCount)
 *
 * @param hWriter
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pCount
 *  - Type: uint64_t*
 *  - [Out] parameter
 *  - Path count
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderPeekCount </B> peeks at the count/index of the next saved
 * video particular to the file name pattern. This is the number that the
 * <count:path> tag will be replaced with.
 */
SC_ERROR SC_API saveRecorderPeekCountPath(saveRecorder hWriter, uint64_t* pCount);

/**
 * @fn SC_ERROR SC_API saveRecorderPeekCountGlobal(saveRecorder hWriter, uint64_t* pCount)
 *
 * @param hWriter
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pCount
 *  - Type: uint64_t*
 *  - [Out] parameter
 *  - Global count
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderPeekCount </B> peeks at the count/index of the next saved
 * video across all image writers and video recorders. This is the number that
 * the <count:global> tag will be replaced with.
 */
SC_ERROR SC_API saveRecorderPeekCountGlobal(saveRecorder hWriter, uint64_t* pCount);

/**
 * @fn SC_ERROR SC_API saveRecorderGetLastFileName(saveRecorder hRecorder, char* pLastFileName, size_t* pLen)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pLastFileName
 *  - Type: char*
 *  - [Out] parameter
 *  - Accepts null
 *  - Last file name used
 *
 * @param pLen
 *  - Type: size_t*
 *  - [In/Out] parameter
 *  - (In) Length of the buffer 
 *  - (Out) Length of the value - including the null terminating character -
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderGetLastFileName </B> retrieves the last file name used to save
 * a video with this context. Returns '' if no file has been saved.
 */
SC_ERROR SC_API saveRecorderGetLastFileName(saveRecorder hRecorder, char* pLastFileName, size_t* pLen);
#ifdef _WIN32
SC_ERROR SC_API saveRecorderGetLastFileNameU(saveRecorder hRecorder, wchar_t* pLastFileName, size_t* pLen);
#endif

/**
 * @fn SC_ERROR SC_API saveRecorderOpen(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderOpen </B> opens the output file, sets up internal buffers,
 * frames, contexts, and settings, and allocates memory to prepare to append
 * input frames.
 *
 * @warning 
 *  - settings cannot be changed until the recorder is closed
 */
SC_ERROR SC_API saveRecorderOpen(saveRecorder hRecorder);

/**
 * @fn SC_ERROR SC_API saveRecorderAppendImage(saveRecorder hRecorder, uint8_t* pImageData)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @param pImageData
 *  - Type: uint8_t*
 *  - [Out] parameter
 *  - Frame data to append
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderAppendImage </B> accepts the next frame's raw image data,
 * encodes it, and appends it to the video file.
 */
SC_ERROR SC_API saveRecorderAppendImage(saveRecorder hRecorder, uint8_t* pImageData);

/**
 * @fn SC_ERROR SC_API saveRecorderClose(saveRecorder hRecorder)
 *
 * @param hRecorder
 *  - Type: saveRecorder
 *  - [In] parameter
 *  - A video recorder
 *
 * @return 
 *  - Type: SC_ERROR
 *  - Error code for the function
 *  - Returns SC_ERR_SUCCESS (0) on success
 *
 * <B> saveRecorderClose </B> cleans up memory and settings of the video,
 * concluding the currently recorded video.
 *
 * @warning 
 *  - additional frames cannot be appended once the file has been closed
 */
SC_ERROR SC_API saveRecorderClose(saveRecorder hRecorder);

#ifdef __cplusplus
}
#endif