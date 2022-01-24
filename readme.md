# VLIB

## Introduction

Vlib is designed to be a C library with some useful modules, such as containers, json/xml/yaml serialization. **For now VLIB is still being developed**.

Although vlib is designed to be a glib-like library, **it is just a personal toy and unable to achieve the performance of glib**. As a result, if you are for a highly-efficient library, vlib is  absolutely not a good choice. However, if you're looking for a get-started C project to learn, try read some codes of vlib, which is well commented and pretty simple.

## Features

### Containers

* [X] **VArray**: An array with dynamic memory allocation
* [X] **VList**: A doubly linked list
* [X] **VStack**: A stack implemented by `VArray`
* [X] **VQueue**: A queue implemented by `VList`
* [ ] **VHashTable**

### Memory

* [ ] **VAllocator**: A memory allocator
* [ ] **VGarbageCollector**: A garbage collector which can be enabled by defining the `__V_ENABLE_GC__` macro

### Algorithms

* [X] **VIterator**
* [ ] **VAlgorithm**: Algorithms implemented with `VIterator`

### Utilities

* [ ] **VJson**: provides the serialization and deserialization of `json`
* [ ] **VXml**: provides the serialization and deserialization of `xml`
* [ ] **VYaml**: provides the serialization and deserialization of `yaml`
* [ ] **VCmdLine**: A command line parser

More details can be found in the comments of the codes.

## Get Started

**VLIB IS NOT READY**

## Contact

If you have any questions, feel free to contact me.

* Email: 1792795667@qq.com
