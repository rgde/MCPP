/**
 *	\file
 */


#pragma once


namespace MCPP {


	/**
	 *	\cond
	 */
	 
	 
	class Server;
	
	
	/**
	 *	\endcond
	 */


}


#include <rleahylib/rleahylib.hpp>
#include <functional>
#include <utility>
#include <thread_pool.hpp>
#include <network.hpp>
#include <data_provider.hpp>
#include <event.hpp>
#include <typedefs.hpp>
#include <http_handler.hpp>
#include <unordered_set>
#include <unordered_map>


namespace MCPP {


	/**
	 *	The direction information is travelling
	 *	over the wire.
	 */
	enum class ProtocolDirection {
	
		ClientToServer,	/**<	From the client to the server.	*/
		ServerToClient,	/**<	From the server to the client.	*/
		Both			/**<	Both	*/
	
	};


	class Server : public Service {
	
	
		private:
		
		
			//	State
			
			//	Is the server running?
			bool running;
			//	Must be held to change the server's state
			Mutex state_lock;
			
			
			//	Data
			
			//	Data provider
			DataProvider * data;
		
		
			//	Components
			Nullable<ConnectionHandler> connections;
			Nullable<ThreadPool> pool;
			Nullable<ModuleLoader> mods;
			
			
			//	For interactive
			
			bool is_interactive;
			
			
			//					//
			//	PRIVATE METHODS	//
			//					//
			inline void server_startup();
			inline void load_mods();
			inline void cleanup_events () noexcept;
			
			
		protected:
		
		
			virtual void OnStart (const Vector<const String> & args) override;
			virtual void OnStop () override;
		
		
		public:
		
		
			static Server & Get () noexcept;
		
		
			/**
			 *	Whether protocol analysis shall be
			 *	performed.
			 *
			 *	Protocol analysis logs all packets
			 *	the server sends and receives, can
			 *	be hard on the logging mechanism,
			 *	and should be enabled only if
			 *	needed.
			 */
			bool ProtocolAnalysis;
			/**
			 *	If ProtocolAnalysis is \em true, the
			 *	direction of the traffic which is to
			 *	be logged.
			 *
			 *	Defaults to ProtocolDirection::Both.
			 */
			ProtocolDirection Direction;
			/**
			 *	If ProtocolAnalysis is \em true, the
			 *	types of packet that shall be logged.
			 */
			std::unordered_set<Byte> LoggedPackets;
			/**
			 *	If ProtocolAnalysis is \em true, and this
			 *	is \em true, all packets shall be logged.
			 *
			 *	Defaults to \em false.
			 */
			bool LogAllPackets;
			/**
			 *	If ProtocolAnalysis is \em true, the
			 *	modules or components that shall emit
			 *	verbose output.
			 */
			std::unordered_set<String> Verbose;
			/**
			 *	If ProtocolAnalysis is \em true, and
			 *	this is \em true, all modules and
			 *	components shall emit verbose output.
			 *
			 *	Defaults to \em false.
			 */
			bool VerboseAll;
			/**
			 *	The maxiumum number of bytes which
			 *	the server shall allow to be buffered
			 *	on a client connection before
			 *	disconnecting them.
			 */
			Word MaximumBytes;
			/**
			 *	The maximum number of players which may
			 *	simultaneously be connected to this
			 *	server.
			 */
			Word MaximumPlayers;
			/**
			 *	Retrieves the server's message of the
			 *	day.
			 *
			 *	\return
			 *		The server's message of the day.
			 */
			String GetMessageOfTheDay ();
			/**
			 *	A listing of the modules loaded by
			 *	the server.
			 */
			Vector<
				Tuple<
					//	Actual name of the module
					String,
					//	File the module was loaded from
					String,
					//	Module handle
					Library
				>
			> Modules;
			/**
			 *	Determines whether the server is running
			 *	in interactive mode or not.
			 *
			 *	\return
			 *		\em true if the server is running
			 *		in interactive mode, \em false
			 *		otherwise.
			 */
			bool IsInteractive () const noexcept;
			/**
			 *	Gets the server's data provider.
			 *
			 *	\return
			 *		A reference to the server's
			 *		data provider
			 */
			DataProvider & Data ();
			/**
			 *	Gets the server's thread pool.
			 *
			 *	\return
			 *		A reference to the server's
			 *		thread pool.
			 */
			ThreadPool & Pool ();
			/**
			 *	A list of clients connected to the
			 *	server.
			 */
			ClientList Clients;
			/**
			 *	Collection of routes to handle incoming
			 *	packets.
			 */
			PacketRouter Router;
			/**
			 *	Returns a string representing the date
			 *	and time the server was built in this
			 *	format: \"MMM DD YYYY HH:MM:SS\".
			 *
			 *	\return
			 *		A string of the above format.
			 */
			String BuildDate () const;
			/**
			 *	Returns a string representing the compiler
			 *	the server was built with.
			 *
			 *	\return
			 *		A string representing the compiler used
			 *		to build the server.
			 */
			String CompiledWith () const;
			
		
			Server (const Server &) = delete;
			Server (Server &&) = delete;
			Server & operator = (const Server &) = delete;
			Server & operator = (Server &&) = delete;
			
			
			/**
			 *	Starts the server in interactive mode.
			 *
			 *	\param [in] args
			 *		The command line arguments to pass
			 *		to the server.
			 */
			void StartInteractive (const Vector<const String> & args);
		
		
			/**
			 *	Creates a new instance of the MCPP
			 *	server.
			 *
			 *	Fails if there is already an
			 *	instance.
			 */
			Server ();
			/**
			 *	Shuts the server down and cleans
			 *	it up.
			 */
			~Server () noexcept;
			
			
			/**
			 *	Writes to the log specific to this
			 *	instance, or the platform default
			 *	if that does not exist or has not
			 *	been loaded.
			 *
			 *	\param [in] message
			 *		The message to log.
			 *	\param [in] type
			 *		The type of message to log.
			 */
			void WriteLog (const String & message, Service::LogType type) noexcept;
			/**
			 *	Writes to the server's chat log.
			 *
			 *	\param [in] from
			 *		The sender of the message to
			 *		log.
			 *	\param [in] to
			 *		A list of recipients to whom
			 *		the message was sent.  If
			 *		empty a broadcast is assumed.
			 *	\param [in] message
			 *		The message-in-question.
			 *	\param [in] notes
			 *		Any notes associated with the
			 *		message, null if there are no
			 *		such notes.
			 */
			void WriteChatLog (const String & from, const Vector<String> & to, const String & message, const Nullable<String> & notes) noexcept;
			
			
			/**
			 *	Should be called when something has
			 *	gone fatally and irrecoverably wrong.
			 *
			 *	Attempts to shut the server down.
			 */
			void Panic () noexcept;
			
			
			/**
			 *	Whether the packet should be logged.
			 *
			 *	\param [in] type
			 *		The type of the packet.
			 *	\param [in] direction
			 *		The direction the packet is moving.
			 *
			 *	\return
			 *		\em true if the packet should be
			 *		logged, \em false otherwise.
			 */
			bool LogPacket (Byte type, ProtocolDirection direction) const noexcept;
			/**
			 *	Whether the module or component given
			 *	should be verbose.
			 *
			 *	\param [in] key
			 *		The key of the module or component
			 *		in question.
			 *
			 *	\return
			 *		\em true if the module or component
			 *		should be verbose, \em false
			 *		otherwise.
			 */
			bool IsVerbose (const String & key) const noexcept;
			
			
			//			//
			//	EVENTS	//
			//			//
			
			
			/**
			 *	Invoked whenever a new connection
			 *	is formed, passed the IP address and
			 *	port from which the remote client is
			 *	connecting.  A return value of \em true
			 *	specifies that the connection should
			 *	be accepted, whereas \em false will
			 *	cause the server to immediately close
			 *	the connection.
			 */
			Event<bool (IPAddress, UInt16)> OnAccept;
			/**
			 *	Invoked whenever a connection is accepted
			 *	into the server, passed the Connection
			 *	object which represents the connection
			 *	and which can be used to send data over
			 *	it.
			 */
			Event<void (SmartPointer<Client>)> OnConnect;
			/**
			 *	Invoked whenever a connection is ended,
			 *	passed the Connection object which
			 *	represents the connection which is about
			 *	to be terminated as well as the reason
			 *	the connection was ended.
			 */
			Event<void (SmartPointer<Client>, const String &)> OnDisconnect;
			/**
			 *	Invoked whenever the server's log is
			 *	written.
			 */
			Event<void (const String &, Service::LogType)> OnLog;
			/**
			 *	Invoked whenever the server's chat log
			 *	is written.
			 */
			Event<void (const String &, const Vector<String> &, const String &, const Nullable<String> &)> OnChatLog;
			/**
			 *	Invoked before and after the server installs
			 *	modules.
			 *
			 *	Useful if the front-end wishes to attach to
			 *	some functionality in the server, basically
			 *	useless otherwise.
			 *
			 *	Passed a boolean which is \em true if the
			 *	callback is being invoked before modules are
			 *	installed, \em false if the callback is being
			 *	invoked after modules are installed.
			 */
			Event<void (bool)> OnInstall;
			/**
			 *	Invoked when any client is logged in.
			 *
			 *	This callback is guaranteed to have ended
			 *	before any further packets from that
			 *	client are dispatched to handlers.
			 */
			Event<void (SmartPointer<Client>)> OnLogin;
			/**
			 *	Invoked before the server shuts down.
			 */
			Event<void ()> OnShutdown;
			/**
			 *	Invoked whenever data is received
			 *	on a connection.  Passed the connection
			 *	on which the data was received, and
			 *	all data currently in that connection's
			 *	buffer.
			 */
			ReceiveCallback OnReceive;
	
	
	};
	
	
	/**
	 *	The currently running/extant instance of the
	 *	MCPP server, or \em null if there is no
	 *	running/extant instance.
	 */
	extern Nullable<Server> RunningServer;


}
