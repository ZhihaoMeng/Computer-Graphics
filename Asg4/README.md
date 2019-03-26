## Info

* CPSC6050    Section:01
  
* Asg04

* CHONG MENG

## Description

* *__aw_t__* class to read .obj file

* Mapping the texture to a pyramid with rotation
  
### function used for debugging

* *__aw_t.getVerts()__* return the vertices coordiates

* *__aw_t.getVi()__* return the indices of vertices 
  

### funtion used for assemble data

* *__aw_t.pack()__* return the pointer of data that will be bound to VBO
  
  * This data may contains texture coordinate and normal coordinate. We can call funtion *__aw_t.isVn()__* and *__aw_t.isVt()__* to get the detail or just judge it by *__aw_t.getVERTEXLength()__* as explained below,
  
* *__aw_t.getDataSize()__* return the size of data in VBO

  
* *__aw_t.getVERTEXLength()__* return the VERTEX length in VBO for stride and offset setting.
  
  
* *__aw_t.getCenter()__* return the center of object
