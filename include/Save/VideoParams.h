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

namespace Save
{
	/**
	 * @class VideoParams
	 *
	 * A container to hold the width, height, and FPS parameters of videos.
	 */
	class SAVE_API VideoParams
	{
	public:
		/**
		 * @fn VideoParams()
		 *
		 * Empty constructor.
		 *
		 * Defaults all members to zero except FPS divisor, which defaults to 1.
		 */
		VideoParams();

		/**
		 * @fn VideoParams(size_t width, size_t height, double fps = 25.0)
		 *
		 * @param width
		 *  - Type: size_t
		 *  - Video width
		 *
		 * @param height
		 *  - Type: size_t
		 *  - Video height
		 *
		 * @param fps
		 *	- Type: double
		 *	- Default: 25.0
		 *	- Frames per second
		 *
		 * A constructor.
		 */
		VideoParams(size_t width, size_t height, double fps = 25.0);

		/**
		 * @fn VideoParams(const VideoParams& params)
		 *
		 * @param params
		 *  - Type: const VideoParams&
		 *  - Video parameters to copy
		 *
		 * A copy constructor
		 */
		VideoParams(const VideoParams& params);

		/**
		 * @fn const VideoParams& operator=(VideoParams params)
		 *
		 * @param params
		 *  - Type: VideoParams
		 *  - Video parameters to copy
		 *
		 * @return 
		 *  - Type: const VideoParams&
		 *  - Copied video parameters
		 *
		 * A copy constructor.
		 */
		const VideoParams& operator=(VideoParams params);

		/**
		 * @fn virtual ~VideoParams()
		 *
		 * A destructor.
		 */
		virtual ~VideoParams();

		/**
		 * @fn virtual void SetWidth(size_t width)
		 *
		 * @param width
		 *  - Type: size_t
		 *  - Width
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetWidth </B> sets the video width.
		 */
		virtual void SetWidth(size_t width);

		/**
		 * @fn virtual void SetHeight(size_t height)
		 *
		 * @param height
		 *  - Type: size_t
		 *  - Height
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetHeight </B> sets the video height.
		 */
		virtual void SetHeight(size_t height);

		/**
		 * @fn virtual void SetFPS(size_t divisor, size_t dividend = 1)
		 *
		 * @param fps
		 *	- Type: double
		 *	- Frames per second
		 *
		 * @return 
		 *  - none
		 *
		 * <B> SetFPS </B> sets the video frames per second.
		 */
		virtual void SetFPS(double fps);

		/**
		 * @fn virtual size_t GetWidth() const
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Video width
		 *
		 * <B> GetWidth </B> retrieves the video width.
		 */
		virtual size_t GetWidth() const;

		/**
		 * @fn virtual size_t GetHeight() const
		 *
		 * @return 
		 *  - Type: size_t
		 *  - Video height
		 *
		 * <B> GetHeight </B> retrieves the video height.
		 */
		virtual size_t GetHeight() const;

		/**
		 * @fn virtual double GetFPS() const
		 *
		 * @return 
		 *  - Type: double
		 *  - Frames per second
		 *
		 * <B> GetFPS </B> calculates frames per second as a floating-point.
		 */
		virtual double GetFPS() const;

	private:
		void* m_pInternal;
	};
}