#pragma once

#include "ArrayRawPointerMap.hpp"
#include "RawMap.hpp"
#include "RawMapIterator.hpp"

#if defined(pankey_Log) && (defined(ArrayRawMap_Log) || defined(pankey_Global_Log) || defined(pankey_Base_Log))
	#include "Logger_status.hpp"
	#define ArrayRawMapLog(status,method,mns) pankey_Log(status,"ArrayRawMap",method,mns)
#else
	#define ArrayRawMapLog(status,method,mns)
#endif

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class K,class V>
			class ArrayRawMap : public ArrayRawPointerMap<K,V>, virtual public RawMap<K,V>{	
				public:
					
					ArrayRawMap(){
						ArrayRawMapLog(pankey_Log_StartMethod, "Constructor", "");
						ArrayRawMapLog(pankey_Log_EndMethod, "Constructor", "");
					}
					
					ArrayRawMap(const ArrayRawMap<K,V>& c_map){
						ArrayRawMapLog(pankey_Log_StartMethod, "Constructor", "");
						this->setOwner(false);
						this->expandLocal(c_map.length());
						
						for(int x = 0; x < c_map.length(); x++){
							K* k = c_map.getKeyPointerByIndex(x);
							V* v = c_map.getValuePointerByIndex(x);
							if(k != nullptr && v != nullptr){
								this->add(*k,*v);
							}
							if(k != nullptr && v == nullptr){
								this->addPointer(*k,v);
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					ArrayRawMap(ArrayRawMap<K,V>&& a_map){
						ArrayRawMapLog(pankey_Log_StartMethod, "Constructor", "");

						this->m_key_owner = a_map.m_key_owner;
						this->m_value_owner = a_map.m_value_owner;
						this->m_index = a_map.m_index;
						this->m_size = a_map.m_size;
						
						this->m_destroyKeyPointer = a_map.m_destroyKeyPointer;
						this->m_destroyValuePointer = a_map.m_destroyValuePointer;
						this->m_reorder = a_map.m_reorder;
						this->m_expandSize = a_map.m_expandSize;

						this->m_keys = a_map.m_keys;
						this->m_values = a_map.m_values;
						
						a_map.m_destroyKeyPointer = nullptr;
						a_map.m_destroyValuePointer = nullptr;

						a_map.m_keys = nullptr;
						a_map.m_values = nullptr;
						
						ArrayRawMapLog(pankey_Log_EndMethod, "Constructor", "");
					}
					
					ArrayRawMap(int c_size) : ArrayRawPointerMap<K,V>(c_size){
						ArrayRawMapLog(pankey_Log_StartMethod, "Constructor", "");
						ArrayRawMapLog(pankey_Log_EndMethod, "Constructor", "");
					}
					
					ArrayRawMap(int c_size, bool c_key_own, bool c_value_own, bool c_reorder) : ArrayRawPointerMap<K,V>(c_size, c_key_own, c_value_own, c_reorder){
						ArrayRawMapLog(pankey_Log_StartMethod, "Constructor", "");
						ArrayRawMapLog(pankey_Log_EndMethod, "Constructor", "");
					}
					
					virtual ~ArrayRawMap(){
						ArrayRawMapLog(pankey_Log_StartMethod, "Destructor", "");
						ArrayRawMapLog(pankey_Log_EndMethod, "Destructor", "");
					}
					
					virtual void addMap(const ArrayRawMap<K,V>& a_map){
						ArrayRawMapLog(pankey_Log_StartMethod, "add", "");
						if(a_map.getSize() < this->length() + a_map.length()){
							this->expandLocal(a_map.length());
						}
						for(int x = 0; x < a_map.length(); x++){
							K* k = a_map.getKeyPointerByIndex(x);
							V* v = a_map.getValuePointerByIndex(x);
							if(k != nullptr && v != nullptr){
								this->add(*k,*v);
							}
							if(k != nullptr && v == nullptr){
								this->addPointer(*k,nullptr);
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "add", "");
					}
					
					virtual RawMapEntry<K,V> add(K a_key, V a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "add", "");
						K* i_key = new K(a_key);
						V* i_value = new V(a_value);
						ArrayRawMapLog(pankey_Log_EndMethod, "add", "");
						return this->addPointers(i_key, i_value);
					}
					
					virtual RawMapEntry<K,V> addPointer(K a_key, V *a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "addPointer", "");
						K* i_key = new K(a_key);
						ArrayRawMapLog(pankey_Log_EndMethod, "addPointer", "");
						return this->addPointers(i_key, a_value);
					}
					
					virtual RawMapEntry<K,V> set(K a_key, V a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "set", "");
						int i_index = this->getKeyIndex(a_key);
						if(i_index == -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "set", "");
							return RawMapEntry<K,V>();
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "set", "");
						return this->setValueByIndex(i_index, a_value);
					}
					
					virtual RawMapEntry<K,V> setPointer(K a_key, V *a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "setPointer", "");
						int i_index = this->getKeyIndex(a_key);
						if(i_index == -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "setPointer", "");
							return RawMapEntry<K,V>();
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "setPointer", "");
						return this->setValuePointerByIndex(i_index, a_value);
					}
					
					virtual RawMapEntry<K,V> setKeyByIndex(int a_position, K a_key){
						ArrayRawMapLog(pankey_Log_StartMethod, "setKeyByIndex", "");
						K* i_key = new K(a_key);
						ArrayRawMapLog(pankey_Log_EndMethod, "setKeyByIndex", "");
						return this->setKeyPointerByIndex(a_position, i_key);
					}
					
					virtual RawMapEntry<K,V> setValueByIndex(int a_position, V a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "setValueByIndex", "");
						V* i_value = new V(a_value);
						ArrayRawMapLog(pankey_Log_EndMethod, "setValueByIndex", "");
						return this->setValuePointerByIndex(a_position, i_value);
					}
					
					virtual bool containKey(K a_key){
						ArrayRawMapLog(pankey_Log_StartMethod, "containKey", "");
						for(int x = 0; x < this->length(); x++){
							K* f_key = this->getKeyPointerByIndex(x);
							if(f_key == nullptr){
								continue;
							}
							if(*f_key == a_key){
								ArrayRawMapLog(pankey_Log_EndMethod, "containKey", "");
								return true;
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "containKey", "");
						return false;
					}
					
					virtual bool containValue(V a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "containValue", "");
						for(int x = 0; x < this->length(); x++){
							V* f_value = this->getValuePointerByIndex(x);
							if(f_value == nullptr){
								continue;
							}
							if(*f_value == a_value){
								ArrayRawMapLog(pankey_Log_EndMethod, "containValue", "");
								return true;
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "containValue", "");
						return false;
					}
					
					virtual K* getKeyPointer(V a_value)const{
						ArrayRawMapLog(pankey_Log_StartMethod, "getKeyPointer", "");
						for(int x = 0; x < this->length(); x++){
							V* f_value = this->getValuePointerByIndex(x);
							if(f_value == nullptr){
								continue;
							}
							if(*f_value == a_value){
								ArrayRawMapLog(pankey_Log_EndMethod, "getKeyPointer", "");
								return this->getKeyPointerByIndex(x);
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "getKeyPointer", "");
						return nullptr;
					}
					
					virtual V* getValuePointer(K a_key)const{
						ArrayRawMapLog(pankey_Log_StartMethod, "getValuePointer", "");
						for(int x = 0; x < this->length(); x++){
							K* f_key = this->getKeyPointerByIndex(x);
							if(f_key == nullptr){
								continue;
							}
							if(*f_key == a_key){
								ArrayRawMapLog(pankey_Log_EndMethod, "getValuePointer", "");
								return this->getValuePointerByIndex(x);
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "getValuePointer", "");
						return nullptr;
					}
					
					virtual RawMapEntry<K,V> removeByKey(K a_key){
						ArrayRawMapLog(pankey_Log_StartMethod, "removeByKey", "");
						int i_position = this->getKeyIndex(a_key);
						if(i_position == -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "removeByKey", "");
							return RawMapEntry<K,V>();
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "removeByKey", "");
						return this->removePointersByIndex(i_position);
					}
					
					virtual RawMapEntry<K,V> removeByValue(V a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "removeByValue", "");
						int i_position = this->getValueIndex(a_value);
						if(i_position == -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "removeByValue", "");
							return RawMapEntry<K,V>();
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "removeByValue", "");
						return this->removePointersByIndex(i_position);
					}
					
					virtual bool destroyByKey(K a_key){
						ArrayRawMapLog(pankey_Log_StartMethod, "destroyByKey", "");
						int i_position = this->getKeyIndex(a_key);
						if(i_position == -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "destroyByKey", "");
							return false;
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "destroyByKey", "");
						return this->destroyByIndex(i_position);
					}
					
					virtual bool destroyByValue(V a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "destroyByValue", "");
						int i_position = this->getValueIndex(a_value);
						if(i_position == -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "destroyByValue", "");
							return false;
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "destroyByValue", "");
						return this->destroyByIndex(i_position);
					}
					
					virtual bool destroy(K a_key){
						ArrayRawMapLog(pankey_Log_StartMethod, "destroy", "");
						int i_position = this->getKeyIndex(a_key);
						if(i_position == -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "destroy", "");
							return false;
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "destroy", "");
						return this->destroyByIndex(i_position);
					}
					
					virtual RawMapEntry<K,V> put(K a_key, V a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "put", "");
						int index = this->getKeyIndex(a_key);
						if(index != -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "put", "index != -1");
							return this->getRawMapEntryByIndex(index);
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "put", "");
						return this->add(a_key, a_value);
					}
					
					virtual RawMapEntry<K,V> putPointer(K a_key, V* a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "putPointer", "");
						int index = this->getKeyIndex(a_key);
						if(index != -1){
							ArrayRawMapLog(pankey_Log_EndMethod, "put", "index != -1");
							return this->getRawMapEntryByIndex(index);
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "putPointer", "");
						return this->addPointer(a_key, a_value);
					}
					
					virtual int getKeyIndex(K a_key){
						ArrayRawMapLog(pankey_Log_StartMethod, "getKeyIndex", "");
						for(int x=0; x < this->length(); x++){
							K* i_key = this->getKeyPointerByIndex(x);
							if(i_key == nullptr){
								continue;
							}
							if(a_key == *i_key){
								ArrayRawMapLog(pankey_Log_EndMethod, "getKeyIndex", x);
								return x;
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "getKeyIndex", "");
						return -1;
					}
					
					virtual int getValueIndex(V a_value){
						ArrayRawMapLog(pankey_Log_StartMethod, "getValueIndex", "");
						for(int x=0; x < this->length(); x++){
							V* i_value = this->getValuePointerByIndex(x);
							if(i_value == nullptr){
								continue;
							}
							if(a_value == *i_value){
								ArrayRawMapLog(pankey_Log_EndMethod, "getValueIndex", "");
								return x;
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "getValueIndex", "");
						return -1;
					}
				
					////////////////////////////////////////////operator part///////////////////////////////////////////////
					
					
					virtual ArrayRawMap<K,V>& operator=(const ArrayRawMap<K,V>& a_map){
						ArrayRawMapLog(pankey_Log_StartMethod, "operator=", "");
						this->clear();
						for(int x = 0; x < a_map.length(); x++){
							K* f_key = this->getKeyPointerByIndex(x);
							V* f_value = this->getValuePointerByIndex(x);
							if(f_key == nullptr || f_value == nullptr){
								continue;
							}
							this->add(*f_key, *f_value);
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "operator=", "");
						return *this;
					}

					virtual ArrayRawMap<K,V>& operator=(ArrayRawMap<K,V>&& a_map){
						ArrayRawMapLog(pankey_Log_StartMethod, "Constructor", "");

						this->m_key_owner = a_map.m_key_owner;
						this->m_value_owner = a_map.m_value_owner;
						this->m_index = a_map.m_index;
						this->m_size = a_map.m_size;
						
						this->m_destroyKeyPointer = a_map.m_destroyKeyPointer;
						this->m_destroyValuePointer = a_map.m_destroyValuePointer;
						this->m_reorder = a_map.m_reorder;
						this->m_expandSize = a_map.m_expandSize;

						this->m_keys = a_map.m_keys;
						this->m_values = a_map.m_values;
						
						a_map.m_destroyKeyPointer = nullptr;
						a_map.m_destroyValuePointer = nullptr;

						a_map.m_keys = nullptr;
						a_map.m_values = nullptr;
						
						ArrayRawMapLog(pankey_Log_EndMethod, "Constructor", "");
						return *this;
					}
					
					virtual bool operator==(const ArrayRawMap<K,V>& a_map){
						ArrayRawMapLog(pankey_Log_StartMethod, "operator==", "");
						if(this->length() != a_map.length()){
							return false;
						}
						for(int x = 0; x < a_map.length(); x++){
							K* f_key_1 = a_map.getKeyPointerByIndex(x);
							V* f_value_1 = a_map.getValuePointerByIndex(x);
							K* f_key_2 = this->getKeyPointerByIndex(x);
							V* f_value_2 = this->getValuePointerByIndex(x);
							if(f_key_1 == f_key_2 && f_value_1 == f_value_2){
								continue;
							}
							if(f_key_1 != nullptr && f_key_2 != nullptr && f_value_1 != nullptr && f_value_2 != nullptr){
								if(*f_key_1 != *f_key_2 || *f_value_1 != *f_value_2){
									return false;
								}
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "operator==", "");
						return true;
					}
					
					virtual bool operator!=(const ArrayRawMap<K,V>& a_map){
						ArrayRawMapLog(pankey_Log_StartMethod, "operator!=", "");
						if(this->length() != a_map.length()){
							return true;
						}
						for(int x = 0; x < a_map.length(); x++){
							K* f_key_1 = a_map.getKeyPointerByIndex(x);
							V* f_value_1 = a_map.getValuePointerByIndex(x);
							K* f_key_2 = this->getKeyPointerByIndex(x);
							V* f_value_2 = this->getValuePointerByIndex(x);
							if(f_key_1 == f_key_2 && f_value_1 == f_value_2){
								continue;
							}
							if(f_key_1 != nullptr && f_key_2 != nullptr && f_value_1 != nullptr && f_value_2 != nullptr){
								if(*f_key_1 != *f_key_2 || *f_value_1 != *f_value_2){
									return true;
								}
							}
						}
						ArrayRawMapLog(pankey_Log_EndMethod, "operator!=", "");
						return false;
					}
				
					////////////////////////////////////////////Iterator part///////////////////////////////////////////////
					
					virtual RawMapIterator<K,V> begin(){
						ArrayRawMapLog(pankey_Log_StartMethod, "begin", this->length());
						ArrayRawMapLog(pankey_Log_EndMethod, "begin", "");
						return RawMapIterator<K,V>(this, 0, this->length());
					}
					virtual RawMapIterator<K,V> end(){
						ArrayRawMapLog(pankey_Log_StartMethod, "end", this->length());
						ArrayRawMapLog(pankey_Log_EndMethod, "end", "");
						return RawMapIterator<K,V>(this, this->length() - 1, this->length());
					}
					
			};

		}

	}

}