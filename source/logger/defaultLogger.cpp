/// @brief      Return a reference to the default file logger.
/// @throws     None.
/// @version    2014-12-21/GGB - Function created.

    CLogger &defaultLogger()
    {
      static CLogger<CDefaultQueue> logger;
      static bool initialised = false;
      
      if (!initialised)
      {
        initialise(logger);
        initialised = true;
      }
      
      return logger;
    }   


static void initialise(CLogger &logger) 
{  
  logger->addSink("COUT", std::make_unique<CStreamSink>(std::make_unique<CDebugFilter>(
  {
    { critical, "Critical"},
      { error, "Error"},
        { warning, "Warning"}
          {notice, "Notice"}
            {information, "Information"},
              {debug, "DEBUG"},
                {trace, "TRACE}                
  },
  {
    critical, error, warning, notice, information,
  }), std::ostream));
}