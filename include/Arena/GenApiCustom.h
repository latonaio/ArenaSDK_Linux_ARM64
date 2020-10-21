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
	 * @class EInterfaceTypeClass
	 *
	 * <B> EInterfaceTypeClass </B> is a static class based on GenApi classes of
	 * the same purpose. It translates from an interface type enum
	 * (GenApi::EInterfaceType) to a string and back. For example, the following
	 * enum value can be translated to and from the following string
	 * representation:
	 *  - enum: GenApi::EInterfaceType::intfIValue
	 *  - string: "intfIValue"
	 *
	 * @warning 
	 *  - Similar classes are found mostly under the GenApi namespace
	 */
	class ARENA_API EInterfaceTypeClass
	{
	public:

		/**
		 * @fn static bool FromString(const GenICam::gcstring& ValueStr, GenApi::EInterfaceType* pValue)
		 *
		 * @param ValueStr
		 *  - Type: const GenICam::gcstring&
		 *  - String representation to translate
		 *
		 * @param pValue
		 *  - Type: GenApi::EInterfaceType*
		 *  - Out parameter
		 *  - Pointer to translated enum value
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if successful
		 *  - Otherwise, false
		 *
		 * <B> FromString </B> translates a string representation of an interface
		 * type enum to its enum value. The second parameter is an out parameter.
		 */
		static bool FromString(const GenICam::gcstring& ValueStr, GenApi::EInterfaceType* pValue);

		/**
		 * @fn static void ToString(GenICam::gcstring& ValueStr, GenApi::EInterfaceType Value)
		 *
		 * @param ValueStr
		 *  - Type: GenICam::gcstring&
		 *  - Pass-by-reference out parameter
		 *  - Translated string representation
		 *  - "intfIUnknown" on failure
		 *
		 * @param Value
		 *  - Type: GenApi::EInterfaceType
		 *  - Enum value to translate
		 *
		 * @return 
		 *  - none
		 *
		 * <B> ToString </B> translates from an interface type enum value to its
		 * string representation. The first parameter is an out parameter.
		 */
		static void ToString(GenICam::gcstring& ValueStr, GenApi::EInterfaceType Value);

		/**
		 * @fn static GenICam::gcstring ToString(GenApi::EInterfaceType Value)
		 *
		 * @param Value
		 *  - Type: GenApi::EInterfaceType
		 *  - Enum value to translate
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - Translated string representation
		 *  - "intfIUnknown" on failure
		 *
		 * <B> ToString </B> translates from an interface type enum value to its
		 * string representation.
		 */
		static GenICam::gcstring ToString(GenApi::EInterfaceType Value);

	private:

		// static class implementation
		// constructor inaccessible
		EInterfaceTypeClass() {};
	};

	/**
	 * @class EIncModeClass
	 *
	 * <B> EIncModeClass </B> is a static class based on GenApi classes of the
	 * same purpose. It translates from an increment mode enum (GenApi::EIncMode)
	 * to a string and back. For example, the following enum value can be
	 * translated to and from the following string representation:
	 *  - enum: GenApi::fixedIncrement
	 *  - string: "fixedIncrement"
	 *
	 * @warning 
	 *  - Similar classes are mostly found under the GenApi namespace
	 */
	class ARENA_API EIncModeClass
	{
	public:

		/**
		 * @fn static bool FromString(const GenICam::gcstring& ValueStr, GenApi::EIncMode* pValue)
		 *
		 * @param ValueStr
		 *  - Type: const GenICam::gcstring&
		 *  - String representation to translate
		 *
		 * @param pValue
		 *  - Type: GenApi::EIncMode*
		 *  - Out parameter
		 *  - Pointer to translated enum value
		 *
		 * @return 
		 *  - Type: bool
		 *  - True if successful
		 *  - Otherwise, false
		 *
		 * <B> FromString </B> translates a string representation of an increment
		 * mode enum to its value. The second parameter is an out parameter.
		 */
		static bool FromString(const GenICam::gcstring& ValueStr, GenApi::EIncMode* pValue);

		/**
		 * @fn static void ToString(GenICam::gcstring& ValueStr, GenApi::EIncMode Value)
		 *
		 * @param ValueStr
		 *  - Type: GenICam::gcstring&
		 *  - Pass-by-reference out parameter
		 *  - Translated string representation
		 *  - "intfIUnknown" on failure
		 *
		 * @param Value
		 *  - Type: GenApi::EIncMode
		 *  - Enum value to translate
		 *
		 * @return 
		 *  - none
		 *
		 * <B> ToString </B> translates from an increment mode enum value to its
		 * string representation. The first parameter is an out parameter.
		 */
		static void ToString(GenICam::gcstring& ValueStr, GenApi::EIncMode Value);

		/**
		 * @fn static GenICam::gcstring ToString(GenApi::EIncMode Value)
		 *
		 * @param Value
		 *  - Type: GenApi::EInterfaceType
		 *  - Enum value to translate
		 *
		 * @return 
		 *  - Type: GenICam::gcstring
		 *  - Translated string representation
		 *  - "intfIUnknown" on failure
		 *
		 * <B> ToString </B> translates from an increment mode enum value to its
		 * string representation.
		 */
		static GenICam::gcstring ToString(GenApi::EIncMode Value);

	private:

		// static class implementation
		// constructor inaccessible
		EIncModeClass() {};
	};

	/**
	 * @fn template<typename T> T GetNodeValue(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& Name)
	 *
	 * @param pNodeMap
	 *  - Type: GenApi::INodeMap*
	 *  - A node map
	 *
	 * @param Name
	 *  - Type: const GenICam::gcstring&
	 *  - Node name
	 *
	 * @return 
	 *  - Type: template<typename T> T
	 *  - Value of the node
	 *  - Template type representation
	 *  - Accepts int64_t, double, bool, GenICam::gcstring
	 *    - Integer nodes use int64_t
	 *    - Float nodes use double
	 *    - Boolean nodes use bool
	 *    - String, enumeration nodes use GenICam::gcstring
	 *
	 * <B> GetNodeValue </B> retrieves a node and gets its value.
	 *
	 * @warning 
	 *  - appropriate for integer, float, boolean, string, and enumeration
	 *    nodes
	 *  - accepts int64_t for integer nodes
	 *  - accepts double for float nodes
	 *  - accepts bool for boolean nodes
	 *  - accepts GenICam::gcstring for string, enumeration nodes
	 */
	template<typename T>
	ARENA_API T GetNodeValue(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& Name);

	extern template ARENA_API bool GetNodeValue<bool>(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iBooleanNodeName);
	extern template ARENA_API double GetNodeValue<double>(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iFloatNodeName);
	extern template ARENA_API int64_t GetNodeValue<int64_t>(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iIntegerNodeName);
	extern template ARENA_API GenICam::gcstring GetNodeValue<GenICam::gcstring>(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iEnumerationNodeName);
	ARENA_API void GetNodeValue(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iRegisterNodeName, uint8_t* pOutputBuffer, int64_t& outputBufferLength);

	/**
	 * @fn template<typename T> void SetNodeValue(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& Name, const T& Value)
	 *
	 * @param pNodeMap
	 *  - Type: GenApi::INodeMap*
	 *  - A node map
	 *
	 * @param Name
	 *  - Type: const GenICam::gcstring&
	 *  - Node name
	 *
	 * @param Value
	 *  - Type: const template<typename T> T&
	 *  - Value to set
	 *  - Template type representation
	 *  - Accepts int64_t, double, bool, GenICam::gcstring:
	 *    - Integer nodes use int64_t
	 *    - Float nodes use double
	 *    - Boolean nodes use bool
	 *    - String, enumeration nodes use GenICam::gcstring*
	 *
	 * @return 
	 *  - none
	 *
	 * <B> SetNodeValue </B> retrieves a node from a node map and sets its value.
	 *
	 * @warning 
	 *  - appropriate for integer, float, boolean, string, and enumeration
	 *    nodes
	 *  - accepts int64_t for integer nodes
	 *  - accepts double for float nodes
	 *  - accepts bool for boolean nodes
	 *  - accepts GenICam::gcstring for string, enumeration nodes
	 */
	template<typename T>
	ARENA_API void SetNodeValue(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& Name, const T& Value);

	extern template ARENA_API void SetNodeValue<bool>(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iBooleanNodeName, const bool& valueToSet);
	extern template ARENA_API void SetNodeValue<double>(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iFloatNodeName, const double& valueToSet);
	extern template ARENA_API void SetNodeValue<int64_t>(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iIntegerNodeName, const int64_t& valueToSet);
	extern template ARENA_API void SetNodeValue<GenICam::gcstring>(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iEnumerationNodeName, const GenICam::gcstring& valueToSet);
	ARENA_API void SetNodeValue(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iRegisterNodeName, const uint8_t* pBuffer, int64_t& length);

	/**
	 * @fn void ExecuteNode(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iCommandNodeName)
	 *
	 * @param pNodeMap
	 *  - Type: GenApi::INodeMap*
	 *  - A node map
	 *
	 * @param iCommandNodeName
	 *  - Type: const GenICam::gcstring&
	 *  - Node name
	 *
	 * <B> ExecuteNode </B> retrieves a node from a node map and executes it.
	 *
	 * @warning 
	 *  - only appropriate for command nodes
	 */
	ARENA_API void ExecuteNode(GenApi::INodeMap* pNodeMap, const GenICam::gcstring& iCommandNodeName);
} // namespace Arena
