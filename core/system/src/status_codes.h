#pragma once

/*!
* \file			status_codes.h
* \brief		Status Codes for Boundless.
* \author		Austin Lynes
* \date			5/50/2024
*/

/*! Status Codes for Engine Processes.. */
enum BReturn {
	ATTEMPT_DELETE_NULLPTR = -3,//!< trying to delete a nullptr, this is not allowed. 
	HANDLE_IS_NULL = -2,		//!< the handle being requested has either not be initllized or is nullptr.
	FAILURE = -1,				//!< the calling method has failed.
	SUCCESS = 0,				//!< the calling method has succeded
};
