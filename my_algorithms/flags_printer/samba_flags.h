#ifndef __SAMBA_FLAGS__
#define __SAMBA_FLAGS__

#define DELETE_ACCESS        (1L<<16)
#define READ_CONTROL_ACCESS  (1L<<17)
#define WRITE_DAC_ACCESS     (1L<<18)
#define WRITE_OWNER_ACCESS   (1L<<19)
#define SYNCHRONIZE_ACCESS   (1L<<20)

#define SYSTEM_SECURITY_ACCESS (1L<<24)
#define MAXIMUM_ALLOWED_ACCESS (1L<<25)
#define GENERIC_ALL_ACCESS     (1<<28)
#define GENERIC_EXECUTE_ACCESS (1<<29)
#define GENERIC_WRITE_ACCESS   (1<<30)
#define GENERIC_READ_ACCESS    (((unsigned)1)<<31)

/* CreateDisposition field. */
#define FILE_SUPERSEDE 0		/* File exists overwrite/supersede. File not exist create. */
#define FILE_OPEN 1			/* File exists open. File not exist fail. */
#define FILE_CREATE 2			/* File exists fail. File not exist create. */
#define FILE_OPEN_IF 3			/* File exists open. File not exist create. */
#define FILE_OVERWRITE 4		/* File exists overwrite. File not exist fail. */
#define FILE_OVERWRITE_IF 5		/* File exists overwrite. File not exist create. */

#endif // __SAMBA_FLAGS__
