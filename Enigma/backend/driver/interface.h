#pragma once
#include "structs.h"

/* typedefs */


typedef struct _IO_STATUS_BLOCK1 {
	union {
		NTSTATUS Status;
		PVOID    Pointer;
	};
	ULONG_PTR Information;
} IO_STATUS_BLOCK1, * PIO_STATUS_BLOCK1;


/* typedefs */

typedef
VOID
( NTAPI* PIO_APC_ROUTINE1 ) (
	IN PVOID ApcContext,
	IN PIO_STATUS_BLOCK1 IoStatusBlock,
	IN ULONG Reserved
	);

extern "C" __int64 DirectIO(
	HANDLE FileHandle,
	HANDLE Event,
	PIO_APC_ROUTINE1
	ApcRoutine,
	PVOID ApcContext,
	PIO_STATUS_BLOCK1 IoStatusBlock,
	ULONG IoControlCode,
	PVOID InputBuffer,
	ULONG InputBufferLength,
	PVOID OutputBuffer,
	ULONG OutputBufferLength );

