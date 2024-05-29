#pragma once

/*!****************************************************************************
*	\file		IMetaDataManager.h
*	\brief		Abstract Application Metadata Management Interface.
*	\author		Austin Lynes
*	\date		5/28/2024
******************************************************************************/

#include "pch.h"


namespace Boundless {
	namespace I {
		struct IMetaData { };
		
		template<typename T>
		class IMetaDataManager {
		public:
			//! @param [in] filepath  path/to/metadata.meta file.
			IMetaDataManager(const std::string& filepath) 
				: metadata_filepath{ filepath }, metadata{T()}
			{ static_assert(std::is_base_of<IMetaData, T>::value && "derived type must be of metadata"); }
			virtual ~IMetaDataManager() = default;

			//! meta data serialization technique. must be provided by client
			virtual BReturn Serialize() = 0;

			//! meta data derialization technique. must be provided by client
			virtual BReturn Deserialize() = 0;

		protected:
			T metadata; //!< the metadta needed for this process

			const std::string metadata_filepath; //!< filepath of the meta file
		};

	}
}