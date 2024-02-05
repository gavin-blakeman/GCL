
enum severity_e
{
  critical,
  error, 
  warning, 
  notice,
  information,
  debug,
  trace,
}

CLogger &defaultLogger();

      // Some inline functions to simplify life

    /// @brief      Function to log a message.
    /// @param[in]  severity: The severity of the message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-06-13/GGB - Converted from macro to function.

    inline void LOGMESSAGE(ESeverity severity, std::string const &message)
    {
      defaultLogger().logMessage(severity, message);
    }

    /// @brief Function to log a critical message.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void CRITICALMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(std::make_unique<CDebugRecord>(critical, message));
    }

    /// @brief      Function to log a critical message.
    /// @param[in]  logger: The logger to use for logging.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-06-13/GGB - Converted from macro to function.

    inline void CRITICALMESSAGE(CLogger &logger, std::string const &message)
    {
      logger.logMessage(critical, message);
    }

    /// @brief      Function to log an error message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-06-13/GGB - Converted from macro to function.

    inline void ERRORMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(error, message);
    }

    /// @brief      Function to log an warning message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-06-13/GGB - Converted from macro to function.

    inline void WARNINGMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(warning, message);
    }

    /// @brief Function to log a notice message.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void NOTICEMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(notice, message);
    }

    /// @brief      Function to log an information message.
    /// @param[in]  message: The message to log.
    /// @throws
    /// @version    2020-06-13/GGB - Converted from macro to function.

    inline void INFOMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(info, message);
    }

    /// @brief Function to log an debug message.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void DEBUGMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(debug, message);
    }

    /// @brief Function to log a trace function entry point.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-13/GGB - Converted from macro to function.

    inline void TRACEMESSAGE(std::string const &message)
    {
      defaultLogger().logMessage(trace, message);
    }

    /// @brief Function to log an exception.
    /// @param[in] message: The message to log.
    /// @throws
    /// @version 2020-06-14/GGB - Converted from macro to function.

    inline void LOGEXCEPTION(std::string const &message)
    {
      defaultLogger().logMessage(exception, message);
    }

    inline void TRACE_ENTER(std::source_location const location = std::source_location::current())
    {
      defaultLogger().logMessage(GCL::logger::trace,
                                 "Entering Function: " + std::string(location.function_name()) +
                                 ". File: " + std::string(location.file_name()) +
                                 ". Line: " + std::to_string(location.line()));
    }

    inline void TRACE_EXIT(std::source_location const location = std::source_location::current())
    {
      defaultLogger().logMessage(GCL::logger::trace,
                                 "Exiting Function: " + std::string(location.function_name()) +
                                 ". File: " + std::string(location.file_name()) +
                                 ". Line: " + std::to_string(location.line()));
    }


    /// @brief      Trace function to trace a line.
    /// @param[in]  location: Source code location.
    /// @throws
    /// @version    2023-10-16/GGB - Function created.

    inline void TRACE_LINE(std::source_location const location = std::source_location::current())
    {
      defaultLogger().logMessage(GCL::logger::trace,
                                 "Passing Line: " + std::string(location.function_name()) +
                                 ". File: " + std::string(location.file_name()) +
                                 ". Line: " + std::to_string(location.line()));
    }

    /// @brief      Trace function to flag an unexpected condition.
    /// @param[in]  location: Source code location.
    /// @throws
    /// @version    2023-10-16/GGB - Function created.

    inline void TRACE_UNEXPECTED(std::source_location const location = std::source_location::current())
    {
      defaultLogger().logMessage(GCL::logger::trace,
                                 "Unexpected branch: " + std::string(location.function_name()) +
                                 ". File: " + std::string(location.file_name()) +
                                 ". Line: " + std::to_string(location.line()));
    }