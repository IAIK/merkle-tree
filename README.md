# Merkle Tree Library

## 1. Introduction

The Merkle Tree Library is a C library implementing a binary [Merkle (hash)
tree] (https://en.wikipedia.org/wiki/Merkle_tree).  The library was initially
developed for use with the [Secure Block Device]
(http://www.iaik.tugraz.at/content/research/opensource/secure_block_device/).
As such it has the following properties:

* Binary hash tree using SHA-256 as hash algorithm
* Supports variable size data stores
  - max number of elements and max hash tree levels are compile-time parameters
  - hash tree grows automatically when new integrity tags (mt_add() function) are appended
  - hash tree shrinks, when number of integrity tags is truncated (mt_truncate() function)

## 2. Building the Merkle Tree Library

The library build system is based on make. We currently do not support a
configure script, if you want to adapt the library to your needs, adapt the
'src/mt_config.h' header file by yourself.

### 2.1 Dependencies

CppUnit - We provide a small set of unit test cases using CppUnit. For
building and running the tests CppUnit is a dependency.

Valgrind - By default the test suite is run with Valgrind's memcheck tool to
help detect memory leaks. Unless use of Valgrind is deactivated, it needs to
be installed.

Doxygen - The library is (sparsely) documented. Doxygen is required to create
the documentation.

### 2.2 Building the Merkle Tree Library

Untar the source, change into the library's root directory, and 'make' it.
Supported targets are:

* debug    - build with debug information enabled
* coverage - build with debugging and coverage support
* release  - build optimzed (-O3, no debug) release version
* doc      - build the documentation (as it is) using Doxygen 
* test     - run the CppUnit based test suite. Call with 'make VGRUN= test' to deactivate Valgrind. 
* clean    - clean up build artifacts

All successful builds create a static library 'libMerkleTree.a' in the 'src'
directory, for linking to other applications.

### 2.3 Comments

This library has so far been tested on ARM (32-bit) and AMD64. 

## 3. Using the Merkle Tree Library

The library's user interface is specified in 'src/merkletree.h'. The
'tests/MerkleTreeTest.cpp' outlines how to use the library. Also the Secure
Block Device uses this library to ensure overall data integrity.  Typically, a
new Merkle Tree instance is created by calling mt_create(). The instance has to
be destroyed by a subsequent call to mt_delete(). In between new integrity tags
can be appended to the tree as leafs by using mt_add(). An existing leaf can be
updated using the mt_update() function. Each addition or update of a leaf will
update the root hash of the hash tree, which can be obtained by calling the
mg_get_root() function. Finally, to reduce the size of the hash tree, the
mt_truncate() function can be used to specify a new last leaf in the tree. All
leaves with a higher index will be invalidated and the memory used to store
them freed. This is useful for example to support a data store that can shrink
and grow in size.

## 4. Licensing

For details on the licensing see LICENSE. The Merkle Tree Library uses the
'sha.h' and 'sha224-256.c' files from
[RFC4634](https://tools.ietf.org/rfc/rfc4634.txt) as implementation of the
SHA-256 hash.

## 5. Bibliography

* https://en.wikipedia.org/wiki/Merkle_tree
* http://andix.iaik.tugraz.at/sbd/
* https://tools.ietf.org/rfc/rfc4634.txt
