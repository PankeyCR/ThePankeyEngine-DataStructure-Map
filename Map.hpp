#pragma once

#if defined(pankey_Log) && (defined(Map_Log) || defined(pankey_Global_Log) || defined(pankey_DataStructure_Map_Log))
	#include "Logger_status.hpp"
	#define MapLog(status,method,mns) pankey_Log(status,"Map",method,mns)
#else
	#define MapLog(status,method,mns)
#endif

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class Policy>
			class Map{
				public:
					virtual ~Map(){
						MapLog(pankey_Log_StartMethod, "Destructor", "");
						MapLog(pankey_Log_EndMethod, "Destructor", "");
					}
					
					virtual bool add(K a_key, V a_value)=0;
					
					virtual bool put(K a_key, V a_value)=0;

					virtual bool set(K a_key, V a_value)=0;
					
					virtual bool contain(K a_key)=0;
					
					virtual V get(K a_key)const=0;
					
					virtual bool remove(K a_key)=0;
			};

		}

	}

}