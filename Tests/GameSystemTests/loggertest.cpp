#include <UnitTest++/UnitTest++.h>

#include "../../WordOfFlat/GameSystem/glog.h"

SUITE(LoggerTests)
{
   TEST(Basic_test)
   {
	    GameLoggerPtr spDefaultLogger;
		
		CHECK(FWG_SUCCEDED(GameLoggerCreator::CreateLogger(spDefaultLogger.OutRef(), NULL)));
		
		FWGLOG_TRACE(wxT("Basic_test::Trace message"), spDefaultLogger);
		FWGLOG_DEBUG(wxT("Basic_test::Debug message"), spDefaultLogger);
		FWGLOG_INFO(wxT("Basic_test::Info message"), spDefaultLogger);
		FWGLOG_WARNING(wxT("Basic_test::Warning message"), spDefaultLogger);
		FWGLOG_ERROR(wxT("Basic_test::Error message"), spDefaultLogger);
		
		FWGLOG_TRACE_FORMAT(wxT("Basic_test:: Format: %s"), spDefaultLogger, wxT("Trace message"), FWGLOG_ENDVAL);
		FWGLOG_DEBUG_FORMAT(wxT("Basic_test:: Format: %s"), spDefaultLogger, wxT("Debug message"), FWGLOG_ENDVAL);
		FWGLOG_INFO_FORMAT(wxT("Basic_test:: Format: %s"), spDefaultLogger, wxT("Info message"), FWGLOG_ENDVAL);
		FWGLOG_WARNING_FORMAT(wxT("Basic_test:: Format: %s"), spDefaultLogger, wxT("Warning message"), FWGLOG_ENDVAL);
		FWGLOG_ERROR_FORMAT(wxT("Basic_test:: Format: %s"), spDefaultLogger, wxT("Error message"), FWGLOG_ENDVAL);
		
		spDefaultLogger.Release();
   }

   TEST(Two_loggers_test)
   {
		GameLoggerPtr spDefaultLogger, spTestLogger;
		
		CHECK(FWG_SUCCEDED(GameLoggerCreator::CreateLogger(spDefaultLogger.OutRef(), wxT("default"))));
		CHECK(FWG_SUCCEDED(GameLoggerCreator::CreateLogger(spTestLogger.OutRef(), wxT("test"))));
		
		FWGLOG_TRACE(wxT("Two_loggers_test::Trace message"), spDefaultLogger);
		FWGLOG_TRACE(wxT("Two_loggers_test::Trace message"), spTestLogger);
		FWGLOG_DEBUG(wxT("Two_loggers_test::Debug message"), spDefaultLogger);
		FWGLOG_DEBUG(wxT("Two_loggers_test::Debug message"), spTestLogger);
		FWGLOG_INFO(wxT("Two_loggers_test::Info message"), spDefaultLogger);
		FWGLOG_INFO(wxT("Two_loggers_test::Info message"), spTestLogger);
		FWGLOG_WARNING(wxT("Two_loggers_test::Warning message"), spDefaultLogger);
		FWGLOG_WARNING(wxT("Two_loggers_test::Warning message"), spTestLogger);
		FWGLOG_ERROR(wxT("Two_loggers_test::Error message"), spDefaultLogger);
		FWGLOG_ERROR(wxT("Two_loggers_test::Error message"), spTestLogger);
		
		FWGLOG_TRACE_FORMAT(wxT("Two_loggers_test:: Format: %s"), spDefaultLogger, wxT("Trace message"), FWGLOG_ENDVAL);
		FWGLOG_TRACE_FORMAT(wxT("Two_loggers_test:: Format: %s"), spTestLogger, wxT("Trace message"), FWGLOG_ENDVAL);
		FWGLOG_DEBUG_FORMAT(wxT("Two_loggers_test:: Format: %s"), spDefaultLogger, wxT("Debug message"), FWGLOG_ENDVAL);
		FWGLOG_DEBUG_FORMAT(wxT("Two_loggers_test:: Format: %s"), spTestLogger, wxT("Debug message"), FWGLOG_ENDVAL);
		FWGLOG_INFO_FORMAT(wxT("Two_loggers_test:: Format: %s"), spDefaultLogger, wxT("Info message"), FWGLOG_ENDVAL);
		FWGLOG_INFO_FORMAT(wxT("Two_loggers_test:: Format: %s"), spTestLogger, wxT("Info message"), FWGLOG_ENDVAL);
		FWGLOG_WARNING_FORMAT(wxT("Two_loggers_test:: Format: %s"), spDefaultLogger, wxT("Warning message"), FWGLOG_ENDVAL);
		FWGLOG_WARNING_FORMAT(wxT("Two_loggers_test:: Format: %s"), spTestLogger, wxT("Warning message"), FWGLOG_ENDVAL);
		FWGLOG_ERROR_FORMAT(wxT("Two_loggers_test:: Format: %s"), spDefaultLogger, wxT("Error message"), FWGLOG_ENDVAL);
		FWGLOG_ERROR_FORMAT(wxT("Two_loggers_test:: Format: %s"), spTestLogger, wxT("Error message"), FWGLOG_ENDVAL);
		
		spDefaultLogger.Release();
		spTestLogger.Release();
   }
   
   
	TEST(Logger_concurency_test)
   {
		GameLoggerPtr spDefaultLogger, spTestLogger;

		CHECK(FWG_SUCCEDED(GameLoggerCreator::CreateLogger(spDefaultLogger.OutRef(), wxT("default"))));
		CHECK(FWG_SUCCEDED(GameLoggerCreator::CreateLogger(spTestLogger.OutRef(), wxT("default"))));
		
		FWGLOG_TRACE(wxT("Logger_concurency_test::Trace message"), spDefaultLogger);
		FWGLOG_TRACE(wxT("Logger_concurency_test::Trace message(2)"), spTestLogger);
		FWGLOG_DEBUG(wxT("Logger_concurency_test::Debug message"), spDefaultLogger);
		FWGLOG_DEBUG(wxT("Logger_concurency_test::Debug message(2)"), spTestLogger);
		FWGLOG_INFO(wxT("Logger_concurency_test::Info message"), spDefaultLogger);
		FWGLOG_INFO(wxT("Logger_concurency_test::Info message(2)"), spTestLogger);
		FWGLOG_WARNING(wxT("Logger_concurency_test::Warning message"), spDefaultLogger);
		FWGLOG_WARNING(wxT("Logger_concurency_test::Warning message(2)"), spTestLogger);
		FWGLOG_ERROR(wxT("Logger_concurency_test::Error message"), spDefaultLogger);
		FWGLOG_ERROR(wxT("Logger_concurency_test::Error message(2)"), spTestLogger);
		
		FWGLOG_TRACE_FORMAT(wxT("Logger_concurency_test:: Format: %s"), spDefaultLogger, wxT("Trace message"), FWGLOG_ENDVAL);
		FWGLOG_TRACE_FORMAT(wxT("Logger_concurency_test:: Format(2): %s"), spTestLogger, wxT("Trace message"), FWGLOG_ENDVAL);
		FWGLOG_DEBUG_FORMAT(wxT("Logger_concurency_test:: Format: %s"), spDefaultLogger, wxT("Debug message"), FWGLOG_ENDVAL);
		FWGLOG_DEBUG_FORMAT(wxT("Logger_concurency_test:: Format(2): %s"), spTestLogger, wxT("Debug message"), FWGLOG_ENDVAL);
		FWGLOG_INFO_FORMAT(wxT("Logger_concurency_test:: Format: %s"), spDefaultLogger, wxT("Info message"), FWGLOG_ENDVAL);
		FWGLOG_INFO_FORMAT(wxT("Logger_concurency_test:: Format(2): %s"), spTestLogger, wxT("Info message"), FWGLOG_ENDVAL);
		FWGLOG_WARNING_FORMAT(wxT("Logger_concurency_test:: Format: %s"), spDefaultLogger, wxT("Warning message"), FWGLOG_ENDVAL);
		FWGLOG_WARNING_FORMAT(wxT("Logger_concurency_test:: Format(2): %s"), spTestLogger, wxT("Warning message"), FWGLOG_ENDVAL);
		FWGLOG_ERROR_FORMAT(wxT("Logger_concurency_test:: Format: %s"), spDefaultLogger, wxT("Error message"), FWGLOG_ENDVAL);
		FWGLOG_ERROR_FORMAT(wxT("Logger_concurency_test:: Format(2): %s"), spTestLogger, wxT("Error message"), FWGLOG_ENDVAL);
		
		spDefaultLogger.Release();
		spTestLogger.Release();
   }
}