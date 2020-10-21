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
	 * @class FeatureStream
	 *
	 * A <B> FeatureStream </B> object allows a device to stream features to and
	 * from files, sharing values amongst one another. Features can be either
	 * written from the device's node map (GenApi::INodeMap) to a file or read
	 * from a file to the node map.
	 *
	 * \code{.cpp}
	 * 	// saving all streamable features from a device to a file
	 * 	{
	 * 		Arena::FeatureStream featureStream(pNodeMap);
	 * 		featureStream.Write("fileName.txt");
	 * 	}
	 * 	// loading all streamable features from a file to a device
	 * 	{
	 * 		Arena::FeatureStream featureStream(pNodeMap);
	 * 		featureStream.Read("fileName.txt");
	 * 	}
	 * \endcode
	 *
	 * Feature streams can stream all streamable features on a device or a select
	 * few. By default, a feature stream will not have had any features selected
	 * and will save or load all available streamable features. For a more
	 * precise list, features may be selected (Arena::FeatureStream::Select,
	 * cascading I/O operator <<).
	 *
	 * \code{.cpp}
	 * 	// saving just width and height to a file
	 * 	// selecting width and height with the output operator
	 * 	{
	 * 		Arena::FeatureStream featureStream(pNodeMap);
	 * 		featureStream << "Width" << "Height";
	 * 		featureStream.Write("fileName.txt");
	 * 	}
	 * \endcode
	 *
	 * Not all features are streamable. If unsure, check feature streamability
	 * (GenApi::INode::IsStreamable).
	 *
	 * \code{.cpp}
	 * 	// checking for streamability, only selecting if streamable
	 * 	{
	 * 		if (pNode->IsStreamable())
	 * 		{
	 * 			featureStream << pNode->GetName();
	 * 		}
	 * 	}
	 * \endcode
	 *
	 * @warning 
	 *  - Not all features are streamable
	 *
	 * @see 
	 *  - Arena::FeatureStream
	 */
	class ARENA_API FeatureStream
	{
	public:

		/**
		 * @fn FeatureStream(GenApi::INodeMap* pNodeMap)
		 *
		 * @param pNodeMap
		 *  - Type: GenApi::INodeMap*
		 *  - Pointer to the node map to stream
		 *
		 * The <B> constructor </B> builds a feature stream from a device's node
		 * map
		 * (GenApi::INodeMap). This node map cannot be changed later; another feature
		 * stream must be created to stream another node map.
		 *
		 * By default, a feature stream will have a flag set to stream all
		 * streamable features. A subset of streamable features may be selected
		 * (Arena::FeatureStream::Select), reversing the flag. If a non-streamable
		 * feature is selected, the call will throw and the flag will remain as
		 * it originally was.
		 *
		 * @see 
		 *  - Arena::FeatureStream::Select
		 */
		FeatureStream(GenApi::INodeMap* pNodeMap);

		/**
		 * @fn virtual void Write(const GenICam::gcstring fileName = "features.txt")
		 *
		 * @param fileName
		 *  - Type: const GenICam::gcstring
		 *  - Default: features.txt
		 *  - Name of the file to write to
		 *
		 * @return 
		 *  - none
		 *
		 * <B> Write </B> streams all selected features from the node map
		 * (GenApi::INodeMap) to a file.
		 *
		 * By default, a feature stream will have a flag set to stream all
		 * streamable features. A subset of streamable features may be selected
		 * (Arena::FeatureStream::Select), reversing the flag. If a non-streamable
		 * feature is selected, the call will throw and the flag will remain as
		 * it originally was.
		 *
		 * Essentially, if no features have been selected, <B> Write </B> will
		 * stream all available streamable features from the node map to the
		 * file; if one or more features have been selected, only the selected
		 * features will be streamed.
		 *
		 * @warning 
		 *  - May throw GenICam::GenericException or other derived exception
		 *  - May throw exception related to file access permissions
		 *
		 * @see 
		 *  - Arena::FeatureStream::Select
		 */
		virtual void Write(const GenICam::gcstring fileName = "features.txt");

		/**
		 * @fn virtual void Read(const GenICam::gcstring fileName = "features.txt")
		 *
		 * @param fileName
		 *  - Type: const GenICam::gcstring
		 *  - Default: features.txt
		 *  - Name of the file to read from
		 *
		 * @return 
		 *  - none
		 *
		 * <B> Read </B> streams all selected features from a file to the node
		 * map
		 * (GenApi::INodeMap).
		 *
		 * Calling <B> Read </B> streams all features from a file. Selecting
		 * features
		 * (Arena::FeatureStream::Select) only applies to <B> Write </B>.
		 *
		 * @warning 
		 *  - May throw GenICam::GenericException or other derived exception
		 *  - May throw exception related to file access permissions
		 *  - Selecting features only applies to write
		 *
		 * @see 
		 *  - Arena::FeatureStream::Select
		 */
		virtual void Read(const GenICam::gcstring fileName = "features.txt");

		/**
		 * @fn virtual void Select(GenICam::gcstring featureName)
		 *
		 * @param featureName
		 *  - Type: GenICam::gcstring
		 *  - Name of the feature to select
		 *
		 * @return 
		 *  - none
		 *
		 * <B> Select </B> adds a single streamable feature to the list of
		 * selected features to stream. If called for the first time, <B> Select
		 * </B> also sets an internal 'select-all' flag to false.
		 *
		 * By default, a feature stream will have a flag set to stream all
		 * streamable features. A subset of streamable features may be selected,
		 * reversing the flag. If a non-streamable feature is selected, the call
		 * will throw and the flag will remain as it originally was.
		 *
		 * Trying to select a non-streamable feature will throw. If unsure, check
		 * streamability:
		 *
		 * \code{.cpp}
		 * 	// checking for streamability, only selecting if streamable
		 * 	{
		 * 		if (pNode->IsStreamable())
		 * 		{
		 * 			featureStream.Select(pNode->GetName());
		 * 		}
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - Provides same functionality as << operator
		 *  - Selecting features only applies to write operation
		 *  - Throws GenICam::InvalidArgumentException if selected feature not
		 *    streamable
		 *  - May throw child of GenICam::GenericExceptio
		 */
		virtual void Select(GenICam::gcstring featureName);

		/**
		 * @fn virtual FeatureStream& operator<<(GenICam::gcstring featureName)
		 *
		 * @param featureName
		 *  - Type: GenICam::gcstring
		 *  - Name of the feature to select
		 *
		 * @return 
		 *  - Type: FeatureStream&
		 *  - Same feature stream object, allowing for cascading
		 *
		 * The <B> cascading I/O operator (<<) </B> adds a single streamable
		 * feature to the list of selected features to stream. If called for the
		 * first time, it also sets an internal 'select-all' flag to false.
		 *
		 * By default, a feature stream has a flag set to stream all streamable
		 * features. A subset of streamable features may be selected
		 * (Arena::FeatureStream::Select), reversing the flag. If a non-streamable
		 * feature is selected, the call will throw and the flag will remain as
		 * it originally was.
		 *
		 * Trying to select a non-streamable feature will throw. If unsure, check
		 * streamability:
		 *
		 * \code{.cpp}
		 * 	// checking for streamability, only selecting if streamable
		 * 	{
		 * 		if (pNode->IsStreamable())
		 * 		{
		 * 			featureStream << pNode->GetName();
		 * 		}
		 * 	}
		 * \endcode
		 *
		 * The operator's ability to cascade calls allows selecting items to be
		 * linked together, one after the other.
		 *
		 * \code{.cpp}
		 * 	// cascading selection calls with the << operator before streaming to file
		 * 	{
		 * 		Arena::FeatureStream featureStream(pNodeMap);
		 * 		featureStream << "Width" << "Height" << "OffsetX" << "OffsetY";
		 * 		featureStream.Write();
		 * 	}
		 * \endcode
		 *
		 * @warning 
		 *  - Provides same functionality as Arena::FeatureStream::Select with
		 *    the added bonus of cascading calls
		 *  - Selecting features only applies to write operation
		 *  - Throws GenICam::InvalidArgumentException if selected feature not
		 *    streamable
		 *  - May throw GenICam::GenericException or other derived exception
		 *
		 * @see 
		 *  - Arena::FeatureStream
		 *  - Arena::FeatureStream::Select
		 */
		virtual FeatureStream& operator<<(GenICam::gcstring featureName);

		/**
		 * @fn virtual ~FeatureStream()
		 *
		 * A destructor
		 */
		virtual ~FeatureStream();

	protected:

		FeatureStream();
		void* m_pData;
	};
}
