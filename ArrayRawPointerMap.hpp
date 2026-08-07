#pragma once

#include "InvokeMethod.hpp"
#include "RawPointerMap.hpp"

#if defined(pankey_Log) && (defined(ArrayRawPointerMap_Log) || defined(pankey_Global_Log) || defined(pankey_Base_Log))
	#include "Logger_status.hpp"
	#define ArrayRawPointerMapLog(status,method,mns) pankey_Log(status,"ArrayRawPointerMap",method,mns)
#else
	#define ArrayRawPointerMapLog(status,method,mns)
#endif

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class K,class V>
			class ArrayRawPointerMap : virtual public RawPointerMap<K,V>{
				public:

					ArrayRawPointerMap(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					ArrayRawPointerMap(const ArrayRawPointerMap<K,V>& c_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");
						this->setOwner(false);
						this->expandLocal(c_map.length());

						for(int x = 0; x < c_map.length(); x++){
							K* k = c_map.getKeyPointerByIndex(x);
							V* v = c_map.getValuePointerByIndex(x);
							this->addPointers(k,v);
							c_map->setRawMapEntryByIndex(x, RawMapEntry<K,V>());
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					ArrayRawPointerMap(ArrayRawPointerMap<K,V>&& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");

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
						
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					ArrayRawPointerMap(int c_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");
						this->expandLocal(c_size);
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					ArrayRawPointerMap(int c_size, bool c_key_own, bool c_value_own, bool c_reorder){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");
						this->setKeyOwner(c_key_own);
						this->setValueOwner(c_value_own);
						this->expandLocal(c_size);
						m_reorder = c_reorder;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					virtual ~ArrayRawPointerMap(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Destructor", "");
						if(this->m_keys != nullptr && this->m_values != nullptr){
							ArrayRawPointerMapLog(pankey_Log_Statement, "Destructor", "this->m_keys != nullptr && this->m_values != nullptr");
							if(this->hasOwner()){
								ArrayRawPointerMapLog(pankey_Log_StartMethod, "Destructor", "this->m_owner");
								for(int x = 0; x < this->length(); x++){
									if(this->m_keys[x] != nullptr && this->isKeyOwner()){
										ArrayRawPointerMapLog(pankey_Log_Statement, "Destructor", "key is deleted");
										this->destroyKeyPointer(this->m_keys[x]);
									}
									if(this->m_values[x] != nullptr && this->isValueOwner()){
										ArrayRawPointerMapLog(pankey_Log_Statement, "Destructor", "key is deleted");
										this->destroyValuePointer(this->m_values[x]);
									}
								}
								ArrayRawPointerMapLog(pankey_Log_StartMethod, "Destructor", "after deleting");
							}
							this->length(0);
							this->setSize(0);
							delete[] this->m_keys;
							delete[] this->m_values;
							this->m_keys = nullptr;
							this->m_values = nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Destructor", "");
					}

					virtual bool isEmpty()const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "isEmpty", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "isEmpty", "");
						return this->length() <= 0 || this->m_keys == nullptr || this->m_values == nullptr;
					}

					virtual RawMapEntry<K,V> addPointers(K* a_key, V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "addPointers", "");
						if(this->length() >= this->getSize()){
							ArrayRawPointerMapLog(pankey_Log_Statement, "addPointers", "this->length() >= this->getSize()");
							this->expandLocal(this->m_expandSize);
						}
						if(this->length() >= this->getSize()){
							ArrayRawPointerMapLog(pankey_Log_Statement, "addPointers", "second chance this->length() >= this->getSize()");
							if(a_key != nullptr && this->isKeyOwner()){
								this->destroyKeyPointer(a_key);
							}
							if(a_value != nullptr && this->isValueOwner()){
								this->destroyValuePointer(a_value);
							}
							return RawMapEntry<K,V>();
						}
						this->m_keys[this->length()] = a_key;
						this->m_values[this->length()] = a_value;
						this->incrementIndex();
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "addPointers", "");
						return RawMapEntry<K,V>(a_key, a_value);
					}

					virtual RawMapEntry<K,V> addRawMapEntry(RawMapEntry<K,V> a_map_entry){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "addRawMapEntry", "");
						if(this->isEmpty()){
							this->expandLocal(this->m_expandSize);
						}
						if(this->length() >= this->getSize()){
							this->expandLocal(this->m_expandSize);
						}
						if(this->length() >= this->getSize()){
							if(a_map_entry.getKey() != nullptr && this->isKeyOwner()){
								a_map_entry.destroyKeyEntry();
							}
							if(a_map_entry.getValue() != nullptr && this->isValueOwner()){
								a_map_entry.destroyValueEntry();
							}
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "addRawMapEntry", "");
							return RawMapEntry<K,V>();
						}
						this->m_keys[this->length()] = a_map_entry.getKey();
						this->m_values[this->length()] = a_map_entry.getValue();
						this->incrementIndex();
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "addRawMapEntry", "");
						return RawMapEntry<K,V>(a_map_entry);
					}

					virtual RawMapEntry<K,V> setPointers(K* a_key, V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setPointers", "");
						int i_index = this->getKeyIndexByPointer(a_key);
						if(i_index == -1){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setPointers", "");
							return RawMapEntry<K,V>();
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setPointers", "");
						return this->setValuePointerByIndex(i_index, a_value);
					}

					virtual RawMapEntry<K,V> setRawMapEntry(RawMapEntry<K,V> a_map_entry){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setRawMapEntry", "");
						int i_index = this->getKeyIndexByPointer(a_map_entry.getKey());
						if(i_index == -1){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setRawMapEntry", "");
							return RawMapEntry<K,V>();
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setRawMapEntry", "");
						return this->setRawMapEntryByIndex(i_index, a_map_entry);
					}

					virtual RawMapEntry<K,V> setKeyPointerByIndex(int a_Index, K* a_key){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setKeyPointerByIndex", "");
						if(a_Index >= this->length()){
							if(a_key != nullptr && this->isKeyOwner()){
								this->destroyKeyPointer(a_key);
							}
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
							return RawMapEntry<K,V>();
						}
						RawMapEntry<K,V> i_entry = this->getRawMapEntryByIndex(a_Index);
						K* i_key = i_entry.getKey();
						if(a_key == i_key){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
							return i_entry;
						}
						if(this->isKeyOwner() && i_key != nullptr){
							this->destroyKeyPointer(i_key);
						}
						this->m_keys[a_Index] = a_key;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
						return RawMapEntry<K,V>(a_key, i_entry.getValue());
					}

					virtual RawMapEntry<K,V> setValuePointerByIndex(int a_Index, V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setValuePointerByIndex", "");
						if(a_Index >= this->length()){
							if(a_value != nullptr && this->isValueOwner()){
								this->destroyValuePointer(a_value);
							}
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
							return RawMapEntry<K,V>();
						}
						RawMapEntry<K,V> i_entry = this->getRawMapEntryByIndex(a_Index);
						V* i_value = i_entry.getValue();
						if(a_value == i_value){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
							return i_entry;
						}
						if(this->isValueOwner() && i_value != nullptr){
							this->destroyValuePointer(i_value);
						}
						this->m_values[a_Index] = a_value;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
						return RawMapEntry<K,V>(i_entry.getKey(), a_value);
					}

					virtual RawMapEntry<K,V> setRawMapEntryByIndex(int a_Index, RawMapEntry<K,V> a_map_entry){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setRawMapEntryByIndex", "");
						if(a_Index >= this->length()){
							if(a_map_entry.getKey() != nullptr && this->isKeyOwner()){
								a_map_entry.destroyKeyEntry();
							}
							if(a_map_entry.getValue() != nullptr && this->isValueOwner()){
								a_map_entry.destroyValueEntry();
							}
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setRawMapEntryByIndex", "a_Index >= this->length()");
							return RawMapEntry<K,V>();
						}
						RawMapEntry<K,V> i_entry = this->getRawMapEntryByIndex(a_Index);
						K* i_key = i_entry.getKey();
						V* i_value = i_entry.getValue();
						if(this->hasOwner()){
							ArrayRawPointerMapLog(pankey_Log_Statement, "setRawMapEntryByIndex", "this->isOwner()");
							if(i_key != nullptr && i_key != a_map_entry.getKey() && this->isKeyOwner()){
								ArrayRawPointerMapLog(pankey_Log_Statement, "setRawMapEntryByIndex", "i_key != nullptr && i_key != a_map_entry.getKey()");
								this->destroyKeyPointer(i_key);
							}
							if(i_value != nullptr && i_value != a_map_entry.getValue() && this->isValueOwner()){
								ArrayRawPointerMapLog(pankey_Log_Statement, "setRawMapEntryByIndex", "i_value != nullptr && i_value != a_map_entry.getValue()");
								this->destroyValuePointer(i_value);
							}
						}
						this->m_keys[a_Index] = a_map_entry.getKey();
						this->m_values[a_Index] = a_map_entry.getValue();
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setRawMapEntryByIndex", "");
						return a_map_entry;
					}

					virtual bool containPairPointers(K* a_key, V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "containPairPointers", "");
						for(int x = 0; x < this->length(); x++){
							if(this->m_keys[x] == a_key && this->m_values[x] == a_value){
								ArrayRawPointerMapLog(pankey_Log_EndMethod, "containPairPointers", "");
								return true;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "containPairPointers", "");
						return false;
					}

					virtual bool containRawMapEntry(RawMapEntry<K,V> a_map_entry){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "containRawMapEntry", "");
						for(int x = 0; x < this->length(); x++){
							if(this->m_keys[x] == a_map_entry.getKey() && this->m_values[x] == a_map_entry.getValue()){
								ArrayRawPointerMapLog(pankey_Log_EndMethod, "containRawMapEntry", "");
								return true;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "containRawMapEntry", "");
						return false;
					}

					virtual bool containKeyByPointer(K* a_key){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "containKeyByPointer", "");
						for(int x = 0; x < this->length(); x++){
							if(this->m_keys[x] == a_key){
								ArrayRawPointerMapLog(pankey_Log_EndMethod, "containKeyByPointer", "");
								return true;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "containKeyByPointer", "");
						return false;
					}

					virtual bool containValueByPointer(V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "containValueByPointer", "");
						for(int x = 0; x < this->length(); x++){
							if(this->m_values[x] == a_value){
								ArrayRawPointerMapLog(pankey_Log_EndMethod, "containValueByPointer", "");
								return true;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "containValueByPointer", "");
						return false;
					}

					virtual V* getValuePointerByPointer(const K* a_key)const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getValuePointerByPointer", "");
						for(int x = 0; x < this->length(); x++){
							if(this->m_keys[x] == a_key){
								ArrayRawPointerMapLog(pankey_Log_EndMethod, "getValuePointerByPointer", "");
								return this->m_values[x];
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getValuePointerByPointer", "");
						return nullptr;
					}

					virtual V* getValuePointerByIndex(int a_Index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getValuePointerByIndex", "");
						if(a_Index >= this->length() || this->isEmpty()){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "getValuePointerByIndex", "");
							return nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getValuePointerByIndex", "");
						return this->m_values[a_Index];
					}

					virtual RawMapEntry<K,V> getRawMapEntryByIndex(int a_Index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getRawMapEntryByIndex", "");
						if(a_Index >= this->length()){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "getRawMapEntryByIndex", "a_Index >= this->length()");
							return RawMapEntry<K,V>();
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getRawMapEntryByIndex", a_Index);
						return RawMapEntry<K,V>(this->m_keys[a_Index], this->m_values[a_Index]);
					}

					virtual K* getKeyPointerByIndex(int a_Index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getKeyPointerByIndex", "");
						if(a_Index >= this->length() || this->isEmpty()){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "getKeyPointerByIndex", "");
							return nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getKeyPointerByIndex", "");
						return this->m_keys[a_Index];
					}

					virtual K* getKeyPointerByPointer(V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getKeyPointerByPointer", "");
						for(int x = 0; x < this->length(); x++){
							if(this->m_values[x] == a_value){
								ArrayRawPointerMapLog(pankey_Log_EndMethod, "getKeyPointerByPointer", "");
								return this->m_keys[x];
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getKeyPointerByPointer", "");
						return nullptr;
					}

					virtual void reset(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "reset", "");
						this->length(0);
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "reset", "");
					}

					virtual void clear(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "clear", "");
						ArrayRawPointerMapLog(pankey_Log_Statement, "clear", "this->length()");
						ArrayRawPointerMapLog(pankey_Log_Statement, "clear", this->length());
						for(int x = 0; x < this->length(); x++){
							ArrayRawPointerMapLog(pankey_Log_Statement, "clear", "this->isOwner()");
							ArrayRawPointerMapLog(pankey_Log_Statement, "clear", this->isOwner());
							if(this->hasOwner()){
								ArrayRawPointerMapLog(pankey_Log_Statement, "clear", "deleting index: ");
								ArrayRawPointerMapLog(pankey_Log_Statement, "clear", x);
								if(this->isKeyOwner()){
									ArrayRawPointerMapLog(pankey_Log_Statement, "clear", "key is deleted");
									this->destroyKeyPointer(this->m_keys[x]);
								}
								if(this->isValueOwner()){
									ArrayRawPointerMapLog(pankey_Log_Statement, "clear", "value is deleted");
									this->destroyValuePointer(this->m_values[x]);
								}
							}
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}
						this->length(0);
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "clear", "");
					}

					virtual void clearKey(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "clearKey", "");
						for(int x = 0; x < this->length(); x++){
							if(this->isKeyOwner()){
								this->destroyKeyPointer(this->m_keys[x]);
							}
							this->m_keys[x] = nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "clearKey", "");
					}

					virtual void clearValue(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "clearValue", "");
						for(int x = 0; x < this->length(); x++){
							if(this->isValueOwner()){
								this->destroyValuePointer(this->m_values[x]);
							}
							this->m_values[x] = nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "clearValue", "");
					}

					virtual RawMapEntry<K,V> removePointersByKeyPointer(K* a_key){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "removePointersByKeyPointer", "");
						if(this->isEmpty()){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByKeyPointer", "");
							return RawMapEntry<K,V>();
						}
						int i_Index = 0;
						for(int x = 0; x < this->length(); x++){
							if(a_key == this->m_keys[x]){
								i_Index = x;
								break;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByKeyPointer", "");
						return this->removePointersByIndex(i_Index);
					}

					virtual RawMapEntry<K,V> removePointersByValuePointer(V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "removePointersByValuePointer", "");
						if(this->isEmpty()){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByValuePointer", "");
							return RawMapEntry<K,V>();
						}
						int i_Index = 0;
						for(int x = 0; x < this->length(); x++){
							if(a_value == this->m_values[x]){
								i_Index = x;
								break;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByValuePointer", "");
						return this->removePointersByIndex(i_Index);
					}

					virtual RawMapEntry<K,V> removePointersByIndex(int a_Index){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "removePointersByIndex", "");
						if(a_Index >= this->length() || this->isEmpty() || a_Index < 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByIndex", "");
							return RawMapEntry<K,V>();
						}
						K* i_key = this->m_keys[a_Index];
						V* i_value = this->m_values[a_Index];
						this->m_keys[a_Index] = nullptr;
						this->m_values[a_Index] = nullptr;
						int i_iteration = this->length();
						this->decrementIndex();
						if(!this->m_reorder){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByIndex", "");
							return RawMapEntry<K,V>(i_key, i_value);
						}
						for(int x = a_Index + 1; x < i_iteration; x++){
							this->m_keys[x - 1] = this->m_keys[x];
							this->m_values[x - 1] = this->m_values[x];
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByIndex", "");
						return RawMapEntry<K,V>(i_key, i_value);
					}

					virtual int getKeyIndexByPointer(K* a_key){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getKeyIndexByPointer", "");
						for(int x = 0; x < this->length(); x++){
							if(a_key == this->m_keys[x]){
								ArrayRawPointerMapLog(pankey_Log_EndMethod, "getKeyIndexByPointer", x);
								return x;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getKeyIndexByPointer", "return -1");
						return -1;
					}

					virtual int getIndexByPointer(V* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getIndexByPointer", "");
						for(int x=0; x < this->length(); x++){
							if(a_value == this->m_values[x]){
								ArrayRawPointerMapLog(pankey_Log_EndMethod, "getIndexByPointer", "");
								return x;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getIndexByPointer", "");
						return -1;
					}

					//resize length by adding more space
					virtual void expandLocal(int a_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "expandLocal", "");
						int i_size = this->getSize() + a_size;
						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocal", "Expanding size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocal", i_size);
						if(i_size <= 1){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocal", "i_size <= 1");
							return;
						}
						K** nK;
						V** nV;
						nK = new K*[i_size];
						nV = new V*[i_size];
						if(this->m_keys != nullptr && this->m_values != nullptr){
							for(int x=0; x < this->getSize(); x++){
								nK[x] = this->m_keys[x];
								nV[x] = this->m_values[x];
							}
						}else if(this->m_keys != nullptr){
							for(int x=0; x < this->getSize(); x++){
								nK[x] = this->m_keys[x];
							}
							delete[] this->m_keys;
							this->m_keys = nullptr;
						}else if(this->m_values != nullptr){
							for(int x=0; x < this->getSize(); x++){
								nV[x] = this->m_values[x];
							}
							delete[] this->m_values;
							this->m_values = nullptr;
						}
						for(int x = this->getSize(); x < i_size; x++){
							nK[x] = nullptr;
							nV[x] = nullptr;
						}
						this->m_keys = nK;
						this->m_values = nV;
						this->m_size = i_size;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocal", "");
					}
					//resize length by adding more space
					// virtual void expandLocal(int a_size){
					// 	ArrayRawPointerMapLog(pankey_Log_StartMethod, "expandLocal", "");
					// 	int i_size = this->getSize() + a_size;
					// 	ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocal", "Expanding size: ");
					// 	ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocal", i_size);
					// 	if(i_size <= 1){
					// 		ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocal", "i_size <= 1");
					// 		return;
					// 	}
					// 	K** nK;
					// 	V** nV;
					// 	nK = new K*[i_size];
					// 	nV = new V*[i_size];
					// 	for(int x=0; x < this->length(); x++){
					// 		nK[x] = this->m_keys[x];
					// 		nV[x] = this->m_values[x];
					// 	}
					// 	if(this->m_keys != nullptr){
					// 		delete[] this->m_keys;
					// 	}
					// 	if(this->m_values != nullptr){
					// 		delete[] this->m_values;
					// 	}
					// 	this->m_keys = nK;
					// 	this->m_values = nV;
					// 	this->m_size = i_size;
					// 	ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocal", "");
					// }

					virtual ArrayRawPointerMap<K,V>* expand(int a_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "expand", "");
						int i_size = this->m_size + a_size;
						ArrayRawPointerMap<K,V> *nprimitive = new ArrayRawPointerMap<K,V>(i_size);
						for(int x = 0; x < this->length(); x++){
							nprimitive->addPointers(this->m_keys[x],this->m_values[x]);
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "expand", "");
						return nprimitive;
					}

					virtual ArrayRawPointerMap<K,V>& operator=(const ArrayRawPointerMap<K,V>& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator=", "const ArrayRawPointerMap<K,V>&");
						this->clear();
						for(int x = 0; x < a_map.length(); x++){
							K* f_key = a_map.getKeyPointerByIndex(x);
							V* f_value = a_map.getValuePointerByIndex(x);
							this->addPointers(f_key, f_value);
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "operator=", "");
						return *this;
					}

					virtual ArrayRawPointerMap<K,V>& operator=(ArrayRawPointerMap<K,V>&& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");

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
						
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
						return *this;
					}

					virtual bool operator==(const ArrayRawPointerMap<K,V>& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator==", "const ArrayRawPointerMap<K,V>&");
						if(a_map.length() != this->length()){
							return false;
						}
						for(int x = 0; x < a_map.length(); x++){
							K* f_key_1 = a_map.getKeyPointerByIndex(x);
							V* f_value_1 = a_map.getValuePointerByIndex(x);
							K* f_key_2 = this->getKeyPointerByIndex(x);
							V* f_value_2 = this->getValuePointerByIndex(x);
							if(f_key_1 != f_key_2 || f_value_1 != f_value_2){
								return false;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "operator==", "");
						return true;
					}

					virtual bool operator!=(const ArrayRawPointerMap<K,V>& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator!=", "const ArrayRawPointerMap<K,V>&");
						if(a_map.length() != this->length()){
							return true;
						}
						for(int x = 0; x < a_map.length(); x++){
							K* f_key_1 = a_map.getKeyPointerByIndex(x);
							V* f_value_1 = a_map.getValuePointerByIndex(x);
							K* f_key_2 = this->getKeyPointerByIndex(x);
							V* f_value_2 = this->getValuePointerByIndex(x);
							if(f_key_1 != f_key_2 || f_value_1 != f_value_2){
								return true;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator!=", "");
						return false;
					}

					virtual void setKeyPointerDestroyer(pankey::Utility::Base::InvokeMethod<K*> a_method){
						m_destroyKeyPointer = a_method;
					}

					virtual void setValuePointerDestroyer(pankey::Utility::Base::InvokeMethod<V*> a_method){
						m_destroyValuePointer = a_method;
					}

				protected:
					virtual void destroyKeyPointer(K* a_pointer){
						if(m_destroyKeyPointer != nullptr){
							pankey::Utility::Base::invoke<K*>(m_destroyKeyPointer, a_pointer);
							return;
						}
						if(a_pointer == nullptr){
							return;
						}
						delete a_pointer;
					}
					
					virtual void destroyValuePointer(V* a_pointer){
						if(m_destroyValuePointer != nullptr){
							pankey::Utility::Base::invoke<V*>(m_destroyValuePointer, a_pointer);
							return;
						}
						if(a_pointer == nullptr){
							return;
						}
						delete a_pointer;
					}
					
					pankey::Utility::Base::InvokeMethod<K*> m_destroyKeyPointer = nullptr;
					pankey::Utility::Base::InvokeMethod<V*> m_destroyValuePointer = nullptr;
					bool m_reorder = true;
					int m_expandSize = 5;

					K** m_keys = nullptr;
					V** m_values = nullptr;
			};
		
		}
		
	}

}
