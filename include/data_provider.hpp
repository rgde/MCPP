/**
 *	\file
 */
 
 
#pragma once


#include <functional>
#include <rleahylib/rleahylib.hpp>


namespace MCPP {
	
	
	/**
	 *	The type of callback that shall be invoked when
	 *	the column saving process is about to begin.
	 *
	 *	When this callback is invoked it means that the
	 *	data provider is about to save the column to the
	 *	backing store.
	 *
	 *	If \em false is returned, the chunk saving process
	 *	shall be immediately aborted.
	 *
	 *	<B>Parameters:</B>
	 *
	 *	1.	X-coordinate of the column.
	 *	2.	Y-coordinate of the column.
	 *	3.	Dimension of the column.
	 */
	typedef std::function<bool (Int32, Int32, SByte)> ColumnSaveBegin;
	/**
	 *	The type of callback that shall be invoked when the
	 *	column saving process completes.
	 *
	 *	<B>Parameters:</B>
	 *
	 *	1.	X-coordinate of the column.
	 *	2.	Y-coordinate of the column.
	 *	3.	Dimension of the column.
	 *	4.	\em true if the save succeeded, \em false
	 *		otherwise.
	 */
	typedef std::function<void (Int32, Int32, SByte, bool)> ColumnSaveEnd;


	/**
	 *	The base class for all implementations
	 *	which supply the server with needed
	 *	data.
	 */
	class DataProvider {
	
	
		public:
		
		
			/**
			 *	Maps a Service::LogType to a descriptive
			 *	string.
			 *
			 *	This function is not compiled into the
			 *	MCPP binary, it must be compiled into
			 *	the data provider binary.
			 *
			 *	Include \em data_provider.cpp in your
			 *	data provider compilation.
			 */
			static String GetLogType (Service::LogType type);
		
		
			DataProvider () noexcept;
			DataProvider (const DataProvider &) = delete;
			DataProvider (DataProvider &&) = delete;
			DataProvider & operator = (const DataProvider &) = delete;
			DataProvider & operator = (DataProvider &&) = delete;
		
		
			/**
			 *	Gets a DataProvider to use for the
			 *	server's data needs.
			 *
			 *	The server shall be linked against
			 *	a certain library which shall provide
			 *	this function, which shall be called
			 *	to dynamically acquire a DataProvider.
			 *
			 *	\return
			 *		A pointer to a DataProvider object.
			 */
			static DataProvider * GetDataProvider ();
			
			
			/**
			 *	Virtual destructor.
			 *
			 *	This function is not compiled into the
			 *	MCPP binary, it must be compiled into
			 *	the data provider binary.
			 *
			 *	Include \em data_provider.cpp in your
			 *	data provider compilation.
			 */
			virtual ~DataProvider () noexcept;
			
			
			/**
			 *	When implemented in a base class, retrieves
			 *	information about this data provider.
			 *
			 *	\return
			 *		A tuple with two members:
			 *
			 *		1.	The name of the data provider.
			 *		2.	A list of tuples which act as
			 *			key/value pairs which map the
			 *			name of a piece of information
			 *			about the provider to the value
			 *			associated with it.
			 */
			virtual Tuple<String,Vector<Tuple<String,String>>> GetInfo () const = 0;
		
		
			/**
			 *	Removes a setting entirely from the
			 *	backing store.
			 *
			 *	\param [in] setting
			 *		A string representing the name
			 *		of the setting to delete.
			 */
			//virtual void DeleteSetting (const String & setting) = 0;
			/**
			 *	Sets a setting in the backing store.
			 *
			 *	\param [in] setting
			 *		A string representing the name
			 *		of the setting to set.
			 *	\param [in] value
			 *		A nullable string representing
			 *		the value to set the setting
			 *		to.
			 */
			//virtual void SetSetting (const String & setting, const Nullable<String> & value) = 0;
			/**
			 *	Attempts to get a setting from
			 *	whatever backing store is in use.
			 *
			 *	If the setting doesn't exist, or
			 *	is explicitly set to some value
			 *	analogous to \em null in the backing
			 *	store, a nulled Nullable<String>
			 *	is returned, otherwise the Nullable<String>
			 *	contains the String whose value corresponds
			 *	to the setting-in-question.
			 *
			 *	\param [in] setting
			 *		The key or name of the setting to
			 *		fetch.
			 *
			 *	\return
			 *		A nullable string which is nulled
			 *		if the setting-in-question is not
			 *		set, otherwise it is set to the value
			 *		of the setting.
			 */
			virtual Nullable<String> GetSetting (const String & setting) = 0;
			
			
			/**
			 *	Retrieves all the values associated with
			 *	the given key in the backing store's
			 *	key/value pair store.
			 *
			 *	\param [in] key
			 *		The key whose values are to be
			 *		retrieved.
			 *
			 *	\return
			 *		A vecter of nullable strings containing
			 *		the values associated with \em key in
			 *		an unspecified order.
			 */
			virtual Vector<Nullable<String>> GetValues (const String & key) = 0;
			/**
			 *	Deletes all key/value pairs with the given
			 *	key and the given value.
			 *
			 *	\param [in] key
			 *		The key that a pair must have to qualify
			 *		for deletion.
			 *	\param [in] value
			 *		The value that a pair must have to qualify
			 *		for deletion.
			 */
			virtual void DeleteValues (const String & key, const String & value) = 0;
			/**
			 *	Creates a new key/value pair in the backing store
			 *	with the given key and value.
			 *
			 *	\param [in] key
			 *		The key associated with the key/value pair
			 *		to be created.
			 *	\param [in] value
			 *		The value associated with the key/value pair
			 *		to be created.
			 */
			virtual void InsertValue (const String & key, const String & value) = 0;
			
			
			/**
			 *	Writes to the log.
			 *
			 *	\param [in] log
			 *		The text to log.
			 *	\param [in] type
			 *		The type of event to log.
			 */
			virtual void WriteLog (const String & log, Service::LogType type) = 0;
			/**
			 *	Writes to the chat log.
			 *
			 *	\param [in] from
			 *		The sender of the message.
			 *	\param [in] to
			 *		A list of the username's of
			 *		the recipients of the message.
			 *	\param [in] message
			 *		The text of the message.
			 *	\param [in] notes
			 *		Optional notes to associate with
			 *		the message.
			 */
			virtual void WriteChatLog (const String & from, const Vector<String> & to, const String & message, const Nullable<String> & notes) = 0;
	
	
	};


}
