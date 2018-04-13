// mycopy.c

// Osmel Savon
//Professor Karen Heart
// 4/9/18

/* mycopy utility
 * makes a copy of a file and assigns the same file
 * permissions to the copy
 * Usage:
 *   ./mycopy <name of original file> <name of copy>
 * If the original file does not exist or the user
 * lacks permission to read it, mycopy emits an error.
 * Also, if a file or directory exists with the name
 * proposed for the copy, mycopy emits an error and
 * terminates.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int error( char * msg )
{
	perror( msg );
	return 2;
}

int usage( char * name )
{
	printf( "Usage: %s <file to copy> <name of copy>\n", name );
	return 1;
}
	
int main( int argc, char * argv[] )
{
	if ( argc != 3 )
		return usage( argv[0] );

	// open files
	int fd = open( argv[1] , O_RDONLY, 0 );
	if (fd == -1) {
		perror(argv[1]);
		return 1;
	}
	int fd2 = creat(argv[2], 0 );
	if (fd2 == -1){
		perror(argv[2]);
		return 1;
	}
	fchmod(fd2, S_IRUSR | S_IWUSR);
	// read bytes from original file and
	char buffer[256];
	int bytes_read;
	do{
		bytes_read = read(fd,buffer, 256);
	// write to copy
		if (bytes_read > 0)
			write(fd2, buffer, 256);
	}while (bytes_read > 0);
	// close files
	close(fd);
	close(fd2);
	return 0;
}
