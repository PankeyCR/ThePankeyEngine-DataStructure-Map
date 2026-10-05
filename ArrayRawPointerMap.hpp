#pragma once

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

			template <class Policy>
			class ArrayRawPointerMap : virtual public RawPointerMap<Policy>{
				public:
					using Key_Type = typename RawPointerMap<Policy>::Key_Type;
					using Value_Type = typename RawPointerMap<Policy>::Value_Type;

					ArrayRawPointerMap(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Constructor", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Constructor", "");
					}

					ArrayRawPointerMap(const ArrayRawPointerMap<Policy>& c_map) = delete;

					ArrayRawPointerMap(ArrayRawPointerMap<Policy>&& a_map) = delete;

					virtual ~ArrayRawPointerMap(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "Destructor", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "Destructor", "");
					}

					virtual bool isEmpty()const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "isEmpty", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "isEmpty", "");
						return this->length() <= 0 || this->m_keys == nullptr || this->m_values == nullptr;
					}

					virtual bool addFastPointers(Key_Type* a_key, Value_Type* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "addFastPointers", "");
						
						this->m_keys[this->length()] = a_key;
						this->m_values[this->length()] = a_value;
						this->incrementIndex();

						this->attachKeyPointer(a_key);
						this->attachValuePointer(a_value);

						ArrayRawPointerMapLog(pankey_Log_EndMethod, "addFastPointers", "");
						return true;
					}

					virtual bool setKeyPointerByIndex(int a_index, Key_Type* a_key){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setKeyPointerByIndex", "");
						if(!this->hasAvailableSize(a_index) || a_key == nullptr){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
							return false;
						}
						Key_Type* i_key = this->getKeyPointerByIndex(a_index);
						if(a_key == i_key){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
							return true;
						}
						
						this->m_keys[a_index] = a_key;

						this->releaseKeyPointer(i_key);
						this->attachKeyPointer(a_key);
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setKeyPointerByIndex", "");
						return true;
					}

					virtual bool setValuePointerByIndex(int a_index, Value_Type* a_value){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "setValuePointerByIndex", "");
						if(!this->hasAvailableSize(a_index)){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
							return false;
						}
						Value_Type* i_value = this->getValuePointerByIndex(a_index);
						if(a_value == i_value){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
							return true;
						}
						
						this->m_values[a_index] = a_value;

						this->releaseValuePointer(i_value);
						this->attachValuePointer(a_value);
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "setValuePointerByIndex", "");
						return true;
					}

					virtual Key_Type* getFastKeyPointerByIndex(int a_index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getFastKeyPointerByIndex", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getFastKeyPointerByIndex", "");
						return this->m_keys[a_index];
					}

					virtual Value_Type* getFastValuePointerByIndex(int a_index) const{
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "getFastValuePointerByIndex", "");
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "getFastValuePointerByIndex", "");
						return this->m_values[a_index];
					}

					virtual void reset(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "reset", "");
						for(int x = 0; x < this->length(); x++){
							this->releaseKeyPointer(this->m_keys[x]);
							this->releaseValuePointer(this->m_values[x]);
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}
						this->m_index = 0;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "reset", "");
					}

					virtual void clear(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "clear", "");
						ArrayRawPointerMapLog(pankey_Log_Statement, "clear", "this->length()");
						ArrayRawPointerMapLog(pankey_Log_Statement, "clear", this->length());
						for(int x = 0; x < this->length(); x++){
							this->releaseKeyPointer(this->m_keys[x]);
							this->releaseValuePointer(this->m_values[x]);
							this->destroyKeyPointer(this->m_keys[x]);
							this->destroyValuePointer(this->m_values[x]);
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}
						this->m_index = 0;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "clear", "");
					}

					virtual void clearValue(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "clearValue", "");
						for(int x = 0; x < this->length(); x++){
							this->releaseValuePointer(this->m_values[x]);
							this->destroyValuePointer(this->m_values[x]);
							this->m_values[x] = nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "clearValue", "");
					}

					virtual bool removePointersByIndex(int a_index){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "removePointersByIndex", "");
						if(a_index >= this->length() || this->isEmpty() || a_index < 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByIndex", "");
							return false;
						}
						Key_Type* i_key = this->m_keys[a_index];
						Value_Type* i_value = this->m_values[a_index];
						this->m_keys[a_index] = nullptr;
						this->m_values[a_index] = nullptr;
						int i_iteration = this->length();
						this->decrementIndex();

						this->releaseKeyPointer(i_key);
						this->releaseValuePointer(i_value);

						for(int x = a_index + 1; x < i_iteration; x++){
							this->m_keys[x - 1] = this->m_keys[x];
							this->m_values[x - 1] = this->m_values[x];
						}
						this->m_keys[i_iteration - 1] = nullptr;
						this->m_values[i_iteration - 1] = nullptr;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "removePointersByIndex", "");
						return true;
					}

					bool shrinkLocalSize(int a_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "shrinkLocalSize", "");

						if(a_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocalSize", "a_size <= 0");
							return false;
						}

						int i_size = this->getSize() - a_size;

						ArrayRawPointerMapLog(pankey_Log_Statement, "shrinkLocalSize", "Shrinking size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "shrinkLocalSize", i_size);

						if(i_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocalSize", "i_size <= 0");
							return false;
						}

						Key_Type** nK = this->createKeyPointerArray(i_size);
						Value_Type** nV = this->createValuePointerArray(i_size);

						int i_new_size = this->m_index < i_size ? this->m_index : i_size;

						for(int x=0; x < i_new_size; x++){
							nK[x] = this->m_keys[x];
							nV[x] = this->m_values[x];
						}
						for(int x = i_new_size; x < i_size; x++){
							nK[x] = nullptr;
							nV[x] = nullptr;
						}

						for(int x = i_new_size; x < this->length(); x++){
							this->releaseKeyPointer(this->m_keys[x]);
							this->releaseValuePointer(this->m_values[x]);
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}

						this->destroyKeyPointerArray(this->m_keys);
						this->destroyValuePointerArray(this->m_values);

						this->m_keys = nK;
						this->m_values = nV;

						this->m_size = i_size;
						this->m_index = i_new_size;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocalSize", "");
						return true;
					}

					bool shrinkLocal(int a_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "shrinkLocal", "");

						if(a_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocal", "a_size <= 0");
							return false;
						}

						int i_size = this->length() - a_size;

						ArrayRawPointerMapLog(pankey_Log_Statement, "shrinkLocal", "Shrinking size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "shrinkLocal", i_size);

						if(i_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocal", "i_size <= 0");
							return false;
						}

						int i_new_size = this->m_index < i_size ? this->m_index : i_size;

						for(int x = i_new_size; x < this->length(); x++){
							this->releaseKeyPointer(this->m_keys[x]);
							this->releaseValuePointer(this->m_values[x]);
							this->m_keys[x] = nullptr;
							this->m_values[x] = nullptr;
						}

						this->m_index = i_new_size;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "shrinkLocal", "");
						return true;
					}

					//resize length by adding more space
					bool expandLocalSize(int a_size){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "expandLocalSize", "");

						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", "Input size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", a_size);

						if(a_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocalSize", "a_size <= 0");
							return false;
						}

						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", "Local size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", this->getSize());

						int i_size = this->getSize() + a_size;

						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", "Expanding size: ");
						ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", i_size);

						if(i_size <= 0){
							ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocalSize", "i_size <= 0");
							return false;
						}

						Key_Type** nK = this->createKeyPointerArray(i_size);
						Value_Type** nV = this->createValuePointerArray(i_size);
						
						for(int x=0; x < this->length(); x++){
							ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", "Re-assinging values");
							ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", "iteration: ");
							ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", x);
							nK[x] = this->m_keys[x];
							nV[x] = this->m_values[x];
						}
						for(int x = this->length(); x < i_size; x++){
							ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", "Nulling values");
							ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", "iteration: ");
							ArrayRawPointerMapLog(pankey_Log_Statement, "expandLocalSize", x);
							nK[x] = nullptr;
							nV[x] = nullptr;
						}

						this->destroyKeyPointerArray(this->m_keys);
						this->destroyValuePointerArray(this->m_values);

						this->m_keys = nK;
						this->m_values = nV;
						this->m_size = i_size;
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "expandLocalSize", "");
						return true;
					}

					void destroyLocal(){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "destroyLocal", "");
						if(this->m_keys != nullptr && this->m_values != nullptr){
							ArrayRawPointerMapLog(pankey_Log_Statement, "destroyLocal", "this->m_keys != nullptr && this->m_values != nullptr");
							for(int x = 0; x < this->length(); x++){
								Key_Type* f_key = this->m_keys[x];
								Value_Type* f_value = this->m_values[x];

								this->releaseKeyPointer(f_key);
								this->releaseValuePointer(f_value);

								this->destroyKeyPointer(f_key);
								this->destroyValuePointer(f_value);
							}
							ArrayRawPointerMapLog(pankey_Log_StartMethod, "destroyLocal", "after deleting");
							this->m_index = 0;
							this->m_size = 0;
							this->destroyKeyPointerArray(this->m_keys);
							this->destroyValuePointerArray(this->m_values);
							this->m_keys = nullptr;
							this->m_values = nullptr;
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "destroyLocal", "");
					}

					ArrayRawPointerMap<Policy>& operator=(const ArrayRawPointerMap<Policy>& a_map) = delete;

					ArrayRawPointerMap<Policy>& operator=(ArrayRawPointerMap<Policy>&& a_map) = delete;

					virtual bool operator==(const ArrayRawPointerMap<Policy>& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator==", "const ArrayRawPointerMap<K,V>&");
						if(a_map.length() != this->length()){
							return false;
						}
						for(int x = 0; x < a_map.length(); x++){
							Key_Type* f_key_1 = a_map.getKeyPointerByIndex(x);
							Value_Type* f_value_1 = a_map.getValuePointerByIndex(x);
							Key_Type* f_key_2 = this->getKeyPointerByIndex(x);
							Value_Type* f_value_2 = this->getValuePointerByIndex(x);
							if(f_key_1 != f_key_2 || f_value_1 != f_value_2){
								return false;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "operator==", "");
						return true;
					}

					virtual bool operator!=(const ArrayRawPointerMap<Policy>& a_map){
						ArrayRawPointerMapLog(pankey_Log_StartMethod, "operator!=", "const ArrayRawPointerMap<K,V>&");
						if(a_map.length() != this->length()){
							return true;
						}
						for(int x = 0; x < a_map.length(); x++){
							Key_Type* f_key_1 = a_map.getKeyPointerByIndex(x);
							Value_Type* f_value_1 = a_map.getValuePointerByIndex(x);
							Key_Type* f_key_2 = this->getKeyPointerByIndex(x);
							Value_Type* f_value_2 = this->getValuePointerByIndex(x);
							if(f_key_1 != f_key_2 || f_value_1 != f_value_2){
								return true;
							}
						}
						ArrayRawPointerMapLog(pankey_Log_EndMethod, "operator!=", "");
						return false;
					}

				protected:
					
					virtual Key_Type** createKeyPointerArray(int a_size){return nullptr;}
					virtual Value_Type** createValuePointerArray(int a_size){return nullptr;}
					
					virtual void destroyKeyPointerArray(Key_Type** a_pointer){}
					virtual void destroyValuePointerArray(Value_Type** a_pointer){}

					Key_Type** m_keys = nullptr;
					Value_Type** m_values = nullptr;
			};
		
		}
		
	}

}
