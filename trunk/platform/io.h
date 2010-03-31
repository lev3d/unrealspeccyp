#ifndef	__IO_H__
#define	__IO_H__

#pragma once

namespace xIo
{

enum { MAX_PATH_LEN = 1024};

void SetResourcePath(const char* resource_path);
const char* ResourcePath(const char* path);

}
//namespace xIo

#endif//__IO_H__
