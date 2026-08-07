#pragma once

#include "RawPointerMap.hpp"

#if defined(pankey_Log) && (defined(RawMap_Log) || defined(pankey_Global_Log) || defined(pankey_Base_Log))
	#include "Logger_status.hpp"
	#define RawMapLog(status,method,mns) pankey_Log(status,"RawMap",method,mns)
#else
	#define RawMapLog(status,method,mns)
#endif

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class K,class V>
			class RawMap : virtual public RawPointerMap<K,V>{
				public:
					virtual ~RawMap(){
						RawMapLog(pankey_Log_StartMethod, "Destructor", "");
						RawMapLog(pankey_Log_EndMethod, "Destructor", "");
					}

					virtual void addCopy(const RawPointerMap<K,V>& a_map){
						RawMapLog(pankey_Log_StartMethod, "addCopy", "");
						for(int x = 0; x < a_map.length(); x++){
							K* k = a_map.getKeyPointerByIndex(x);
							V* v = a_map.getValuePointerByIndex(x);
							if(k == nullptr || v == nullptr){
								continue;
							}
							this->add(*k,*v);
						}
						RawMapLog(pankey_Log_EndMethod, "addCopy", "");
					}
					
					virtual RawMapEntry<K,V> add(K a_key, V a_value)=0;
					virtual RawMapEntry<K,V> addPointer(K a_key, V* a_value)=0;

					virtual RawMapEntry<K,V> set(K a_key, V a_value)=0;
					virtual RawMapEntry<K,V> setPointer(K a_key, V* a_value)=0;
					
					virtual RawMapEntry<K,V> setKeyByIndex(int a_position, K a_key)=0;
					virtual RawMapEntry<K,V> setValueByIndex(int a_position, V a_value)=0;
					
					virtual bool containKey(K a_key)=0;
					virtual bool containValue(V a_value)=0;
					
					virtual K* getKeyPointer(V a_value)const=0;
					
					virtual V* getValuePointer(K a_key)const=0;
					
					virtual V get(K a_key)const{
						RawMapLog(pankey_Log_StartMethod, "get", "");
						V* i_value = this->getValuePointer(a_key);
						if(i_value == nullptr){
							RawMapLog(pankey_Log_EndMethod, "get", "");
							return V();
						}
						RawMapLog(pankey_Log_EndMethod, "get", "");
						return *i_value;
					}
					virtual K getKeyByIndex(int a_position){
						RawMapLog(pankey_Log_StartMethod, "getKeyByIndex", "");
						K* i_key = this->getKeyPointerByIndex(a_position);
						if(i_key == nullptr){
							RawMapLog(pankey_Log_EndMethod, "getKeyByIndex", "");
							return K();
						}
						RawMapLog(pankey_Log_EndMethod, "getKeyByIndex", "");
						return *i_key;
					}
					virtual V getValueByIndex(int a_position){
						RawMapLog(pankey_Log_StartMethod, "getValueByIndex", "");
						V* i_value = this->getValuePointerByIndex(a_position);
						if(i_value == nullptr){
							RawMapLog(pankey_Log_EndMethod, "getValueByIndex", "");
							return V();
						}
						RawMapLog(pankey_Log_EndMethod, "getValueByIndex", "");
						return *i_value;
					}
					
					virtual RawMapEntry<K,V> removeByKey(K a_key)=0;
					virtual RawMapEntry<K,V> removeByValue(V a_value)=0;
					
					virtual bool destroyByKey(K a_key)=0;
					virtual bool destroyByValue(V a_value)=0;
					
					virtual bool destroy(K a_key)=0;
					
					virtual RawMapEntry<K,V> put(K a_key, V a_value)=0;
					virtual RawMapEntry<K,V> putPointer(K a_key, V* a_value)=0;
					
					virtual int getKeyIndex(K a_key)=0;
					
					virtual int getValueIndex(V a_key)=0;
					
					template<class... Args>
					void addKeyPack(Args... a_values){
						RawMapLog(pankey_Log_StartMethod, "addKeyPack", "");
						K i_array[] = {a_values...};
						for(const K& k : i_array){
							this->addPointers(new K(k), new V());
						}
						RawMapLog(pankey_Log_EndMethod, "addKeyPack", "");
					}
					
					template<class... Args>
					void addValuePack(Args... a_values){
						RawMapLog(pankey_Log_StartMethod, "addValuePack", "");
						V i_array[] = {a_values...};
						for(const V& v : i_array){
							this->addPointers(new K(), new V(v));
						}
						RawMapLog(pankey_Log_EndMethod, "addValuePack", "");
					}
					
					template<class... Args>
					void addKeyPack(V v, Args... a_values){
						RawMapLog(pankey_Log_StartMethod, "addKeyPack", "");
						K i_array[] = {a_values...};
						for(const K& k : i_array){
							this->addPointers(new K(k), new V(v));
						}
						RawMapLog(pankey_Log_EndMethod, "addKeyPack", "");
					}
					
					template<class... Args>
					void addValuePack(K k, Args... a_values){
						RawMapLog(pankey_Log_StartMethod, "addValuePack", "");
						V i_array[] = {a_values...};
						for(const V& v : i_array){
							this->addPointers(new K(k), new V(v));
						}
						RawMapLog(pankey_Log_EndMethod, "addValuePack", "");
					}
			};

		}

	}

}