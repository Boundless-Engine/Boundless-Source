#pragma once

/*!
* \file			status_codes.h
* \brief		Status Codes for Boundless.
* \author		Austin Lynes
* \date			5/50/2024
*/

/*! Status Codes for Engine Processes.. */
enum BReturn {
	ARGUMENT_ERROR		= -6,	//!< a required argument is incorrect.
	ATTEMPT_DELETE_NULLPTR = -5,//!< trying to delete a nullptr, this is not allowed. 
	HANDLE_IS_NULL = -4,		//!< the handle being requested has either not be initllized or is nullptr.
	NO_IMPL_FAILURE = -3,		//!< the portion of the process you are trying to access is not implemented and this is triggered as a failing call.
	FAILURE = -1,				//!< the calling method has failed.
	SUCCESS = 0,				//!< the calling method has succeded
	NO_IMPL_SUCCESS = 0,			//!< the portion of the process you are trying to access is not implemented and this is triggered as a succeding call.
};
