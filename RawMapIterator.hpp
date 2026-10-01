#pragma once

#include "RawPointerMap.hpp"

namespace pankey{

	namespace DataStructure{

		namespace Map{

			template <class Policy>
			class RawMapIterator{
				public:
					using Key_Type = typename Policy::Key_Type;
					using Value_Type = typename Policy::Value_Type;

					using Size_Type = typename Policy::Size_Type;

					int position = 0;
					int size = 0;
					RawPointerMap<Policy>* m_map = nullptr;
					
					RawMapIterator(RawPointerMap<Policy>* i_map, int p, int s){
						m_map = i_map;
						position = p;
						size = s;
					}
					
					RawMapIterator(const RawMapIterator<Policy>& entry){
						m_map = entry.m_map;
						position = entry.position;
						size = entry.size;
					}
					
					RawMapIterator(RawMapIterator<Policy>&& entry){
						m_map = entry.m_map;
						position = entry.position;
						size = entry.size;
					}
					
					RawMapIterator& operator=(const RawMapIterator<Policy>& entry){
						m_map = entry.m_map;
						position = entry.position;
						size = entry.size;
						return *this;
					}
					
					RawMapIterator& operator=(RawMapIterator<Policy>&& entry){
						m_map = entry.m_map;
						position = entry.position;
						size = entry.size;
						return *this;
					}
					
					bool operator!=(const RawMapIterator<Policy>& entry){
						return position != entry.size;
					}
					
					bool operator==(const RawMapIterator<Policy>& entry){
						return position == entry.size;
					}
					
					virtual ~RawMapIterator(){
					}
					
					virtual bool isValid(){
						return m_map != nullptr;
					}
					
					virtual Key_Type getKey(){
						Key_Type* key = m_map->getKeyPointerByIndex(position);
						if(key == nullptr){
							return Key_Type();
						}
						return *key;
					}
					
					virtual Key_Type* getKeyPointer(){
						return m_map->getKeyPointerByIndex(position);
					}
					
					virtual Value_Type getValue(){
						Value_Type* value = m_map->getValuePointerByIndex(position);
						if(value == nullptr){
							return Value_Type();
						}
						return *value;
					}
					
					virtual Value_Type* getValuePointer(){
						return m_map->getValuePointerByIndex(position);
					}
					
					virtual void setKey(Key_Type k){
						Key_Type* key = m_map->getKeyPointerByIndex(position);
						if(key == nullptr){
							return;
						}
						*key = k;
					}
					
					virtual void setValue(Value_Type v){
						Value_Type* value = m_map->getValuePointerByIndex(position);
						if(value == nullptr){
							return;
						}
						*value = v;
					}
						
					virtual void operator ++(){
						this->position++;
					}
					virtual RawMapIterator<Policy> operator *(){
						return RawMapIterator<Policy>(m_map,position,size);
					}
			};

		}

	}

}