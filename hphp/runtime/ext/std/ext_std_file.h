/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
   | Copyright (c) 1997-2010 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#pragma once

#include "hphp/runtime/ext/extension.h"
#include "hphp/runtime/ext/stream/ext_stream.h"

// To get the values of the SEEK constants
#include <stdio.h>

#undef basename

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////
// constants

#define PHP_FILE_USE_INCLUDE_PATH   1
#define PHP_FILE_IGNORE_NEW_LINES   2
#define PHP_FILE_SKIP_EMPTY_LINES   4
#define PHP_FILE_APPEND             8
#define PHP_FILE_NO_DEFAULT_CONTEXT 16

#ifndef GLOB_ONLYDIR
# define GLOB_ONLYDIR (1<<30)
# define GLOB_EMULATE_ONLYDIR
# define GLOB_FLAGMASK (~GLOB_ONLYDIR)
#else
# define GLOB_FLAGMASK (~0)
#endif

#define PHP_GLOB_FLAGS (0 | GLOB_BRACE | GLOB_MARK  \
                          | GLOB_NOSORT | GLOB_NOCHECK \
                          | GLOB_NOESCAPE | GLOB_ERR \
                          | GLOB_ONLYDIR)
#define PHP_GLOB_FLAGMASK (GLOB_FLAGMASK & PHP_GLOB_FLAGS)

#ifdef _MSC_VER
const StaticString s_DIRECTORY_SEPARATOR("\\");
const StaticString s_PATH_SEPARATOR(";");
#else
const StaticString s_DIRECTORY_SEPARATOR("/");
const StaticString s_PATH_SEPARATOR(":");
#endif
const int64_t k_LOCK_SH = 1;
const int64_t k_LOCK_EX = 2;
const int64_t k_LOCK_UN = 3;
const int64_t k_LOCK_NB = 4;
const int64_t k_SCANDIR_SORT_ASCENDING = 0;
const int64_t k_SCANDIR_SORT_DESCENDING = 1;
const int64_t k_SCANDIR_SORT_NONE = 2;

constexpr int64_t k_INI_SCANNER_NORMAL = 0;
constexpr int64_t k_INI_SCANNER_RAW = 1;

///////////////////////////////////////////////////////////////////////////////
// file handle based file operations

Variant HHVM_FUNCTION(fopen,
                      const String& filename,
                      const String& mode,
                      bool use_include_path = false,
                      const Variant& context = uninit_null());
Variant HHVM_FUNCTION(popen,
                      const String& command,
                      const String& mode);
bool HHVM_FUNCTION(fclose,
                   const Resource& handle);
Variant HHVM_FUNCTION(pclose,
                      const Variant& handle);
Variant HHVM_FUNCTION(fseek,
                      const Resource& handle,
                      int64_t offset,
                      int64_t whence = SEEK_SET);
bool HHVM_FUNCTION(rewind,
                   const Resource& handle);
Variant HHVM_FUNCTION(ftell,
                      const Resource& handle);
bool HHVM_FUNCTION(feof,
                   const Resource& handle);
Variant HHVM_FUNCTION(fstat,
                      const Resource& handle);
Variant HHVM_FUNCTION(fread,
                      const Resource& handle,
                      int64_t length);
Variant HHVM_FUNCTION(fgetc,
                      const Resource& handle);
Variant HHVM_FUNCTION(fgets,
                      const Resource& handle,
                      int64_t length = 0);
Variant HHVM_FUNCTION(fgetss,
                      const Resource& handle,
                      int64_t length = 0,
                      const String& allowable_tags = null_string);
Variant HHVM_FUNCTION(fscanf,
                      const Resource& handle,
                      const String& format);
Variant HHVM_FUNCTION(fpassthru,
                      const Resource& handle);
Variant HHVM_FUNCTION(fwrite,
                      const Resource& handle,
                      const String& data,
                      int64_t length = 0);
Variant HHVM_FUNCTION(fputs,
                      const Resource& handle,
                      const String& data,
                      int64_t length = 0);
Variant HHVM_FUNCTION(fprintf,
                      const Variant& handle,
                      const String& format,
                      const Array& args = null_array);
Variant HHVM_FUNCTION(vfprintf,
                      const Variant& handle,
                      const Variant& format,
                      const Variant& args);
bool HHVM_FUNCTION(fflush,
                   const Resource& handle);
bool HHVM_FUNCTION(ftruncate,
                   const Resource& handle,
                   int64_t size);
bool HHVM_FUNCTION(flock,
                   const Resource& handle,
                   int64_t operation,
                   bool& wouldblock);
Variant HHVM_FUNCTION(fputcsv,
                      const Resource& handle,
                      const Array& fields,
                      const String& delimiter = ",",
                      const String& enclosure = "\"",
                      const String& escape = "\\");
Variant HHVM_FUNCTION(fgetcsv,
                      const Resource& handle,
                      int64_t length = 0,
                      const String& delimiter = ",",
                      const String& enclosure = "\"",
                      const String& escape = "\\");

///////////////////////////////////////////////////////////////////////////////
// file name based file operations

Variant HHVM_FUNCTION(file_get_contents,
                      const String& filename,
                      bool use_include_path = false,
                      const Variant& context = uninit_null(),
                      int64_t offset = -1,
                      int64_t maxlen = -1);
Variant HHVM_FUNCTION(file_put_contents,
                      const String& filename,
                      const Variant& data,
                      int64_t flags = 0,
                      const Variant& context = uninit_null());
Variant HHVM_FUNCTION(file,
                      const String& filename,
                      int64_t flags = 0,
                      const Variant& context = uninit_null());
Variant HHVM_FUNCTION(readfile,
                      const String& filename,
                      bool use_include_path = false,
                      const Variant& context = uninit_null());
bool HHVM_FUNCTION(move_uploaded_file,
                   const String& filename,
                   const String& destination);
Variant HHVM_FUNCTION(parse_ini_file,
                      const String& filename,
                      bool process_sections = false,
                      int64_t scanner_mode = k_INI_SCANNER_NORMAL);
Variant HHVM_FUNCTION(parse_ini_string,
                      const String& ini,
                      bool process_sections = false,
                      int64_t scanner_mode = k_INI_SCANNER_NORMAL);
Variant HHVM_FUNCTION(md5_file,
                      const String& filename,
                      bool raw_output = false);
Variant HHVM_FUNCTION(sha1_file,
                      const String& filename,
                      bool raw_output = false);

///////////////////////////////////////////////////////////////////////////////
// shell commands

bool HHVM_FUNCTION(chmod,
                   const String& filename,
                   int64_t mode);
bool HHVM_FUNCTION(chown,
                   const String& filename,
                   const Variant& user);
bool HHVM_FUNCTION(lchown,
                   const String& filename,
                   const Variant& user);
bool HHVM_FUNCTION(chgrp,
                   const String& filename,
                   const Variant& group);
bool HHVM_FUNCTION(lchgrp,
                   const String& filename,
                   const Variant& group);
bool HHVM_FUNCTION(touch,
                   const String& filename,
                   int64_t mtime = 0,
                   int64_t atime = 0);
bool HHVM_FUNCTION(copy,
                   const String& source,
                   const String& dest,
                   const Variant& context = uninit_null());
bool HHVM_FUNCTION(rename,
                   const String& oldname,
                   const String& newname,
                   const Variant& context = uninit_null());
int64_t HHVM_FUNCTION(umask,
                      const Variant& mask = uninit_variant);
bool HHVM_FUNCTION(unlink,
                   const String& filename,
                   const Variant& context = uninit_null());
bool HHVM_FUNCTION(link,
                   const String& target,
                   const String& link);
bool HHVM_FUNCTION(symlink,
                   const String& target,
                   const String& link);
String HHVM_FUNCTION(basename,
                     const String& path,
                     const String& suffix = null_string);
bool HHVM_FUNCTION(fnmatch,
                   const String& pattern,
                   const String& filename,
                   int64_t flags = 0);
Variant HHVM_FUNCTION(glob,
                      const String& pattern,
                      int64_t flags = 0);
Variant HHVM_FUNCTION(tempnam,
                      const String& dir,
                      const String& prefix);
Variant HHVM_FUNCTION(tmpfile);

///////////////////////////////////////////////////////////////////////////////
// stats functions

Variant HHVM_FUNCTION(fileperms,
                      const String& filename);
Variant HHVM_FUNCTION(fileinode,
                      const String& filename);
Variant HHVM_FUNCTION(filesize,
                      const String& filename);
Variant HHVM_FUNCTION(fileowner,
                      const String& filename);
Variant HHVM_FUNCTION(filegroup,
                      const String& filename);
Variant HHVM_FUNCTION(fileatime,
                      const String& filename);
Variant HHVM_FUNCTION(filemtime,
                      const String& filename);
Variant HHVM_FUNCTION(filectime,
                      const String& filename);
Variant HHVM_FUNCTION(filetype,
                      const String& filename);
Variant HHVM_FUNCTION(linkinfo,
                      const String& filename);
bool HHVM_FUNCTION(is_writable,
                   const String& filename);
bool HHVM_FUNCTION(is_writeable,
                   const String& filename);
bool HHVM_FUNCTION(is_readable,
                   const String& filename);
bool HHVM_FUNCTION(is_executable,
                   const String& filename);
bool HHVM_FUNCTION(is_file,
                   const String& filename);
bool HHVM_FUNCTION(is_dir,
                   const String& filename);
bool HHVM_FUNCTION(is_link,
                   const String& filename);
bool HHVM_FUNCTION(is_uploaded_file,
                   const String& filename);
bool HHVM_FUNCTION(file_exists,
                   const String& filename);
Variant HHVM_FUNCTION(stat,
                      const String& filename);
Variant HHVM_FUNCTION(lstat,
                      const String& filename);
void HHVM_FUNCTION(clearstatcache,
                   bool clear_realpath_cache = false,
                   const Variant& filename = uninit_variant);
Variant HHVM_FUNCTION(readlink_internal,
                      const String& path,
                      bool warning_compliance);
Variant HHVM_FUNCTION(readlink,
                      const String& path);
Variant HHVM_FUNCTION(realpath,
                      const String& path);
Variant HHVM_FUNCTION(pathinfo,
                      const String& path,
                      int64_t opt = 15);
Variant HHVM_FUNCTION(disk_free_space,
                      const String& directory);
Variant HHVM_FUNCTION(diskfreespace,
                      const String& directory);
Variant HHVM_FUNCTION(disk_total_space,
                      const String& directory);

///////////////////////////////////////////////////////////////////////////////
// directory functions

bool HHVM_FUNCTION(mkdir,
                   const String& pathname,
                   int64_t mode = 0777,
                   bool recursive = false,
                   const Variant& context = uninit_null());
bool HHVM_FUNCTION(rmdir,
                   const String& dirname,
                   const Variant& context = uninit_null());
String HHVM_FUNCTION(dirname,
                     const String& path);
Variant HHVM_FUNCTION(getcwd);
bool HHVM_FUNCTION(chdir,
                   const String& directory);
bool HHVM_FUNCTION(chroot,
                   const String& directory);
Variant HHVM_FUNCTION(dir,
                      const String& directory);
Variant HHVM_FUNCTION(opendir,
                      const String& path,
                      const Variant& context = uninit_null());
Variant HHVM_FUNCTION(readdir,
                      const Variant& dir_handle = uninit_variant);
void HHVM_FUNCTION(rewinddir,
                   const Variant& dir_handle = uninit_variant);
Variant HHVM_FUNCTION(scandir,
                      const String& directory,
                      bool descending = false,
                      const Variant& context = uninit_null());
void HHVM_FUNCTION(closedir,
                   const Variant& dir_handle = uninit_variant);

///////////////////////////////////////////////////////////////////////////////
}
