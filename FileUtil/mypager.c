// mypager.c



/* mypager utility
 * Prints a file to standard output, one page worth of lines
 * at a time. It is designed for text files because it prints
 * each byte to the screen as an ASCII character.
 * The user controls the output by pressing keys, as follows:
 * 'f': forwards to the next page
 * 'q': quits
 * NOTE: Each keypress is read immediately; the user does not
 * press the Enter key. To learn how immediate input mode is
 * effectuated, see the eliminate_stdio_buffering() function
 * (below) or see 'man termios'.
 */
 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>

// preprocessor definitions
#define PAGE_SIZE 20
#define LINE_WIDTH 80

// forward declarations
void display_page();
int fetch_next_line( char line[] );
int fetch_next_word( char word[], int max_size );
int refill_buffer( int start );

void eliminate_stdio_buffering();
void restore_stdio_buffering();

// global variables
int fd;
struct termios old, new;
bool no_data;
int start_byte, last_byte, bytes_in_buf;
char buffer[1600];

int usage( char * name )
{
	fprintf( stderr, "Usage:\n" );
	fprintf( stderr, "%s <filename>\n", name );
	return 1;
}

/* void display_page()
 * Calls fetch_next_line() until either:
 * a) all of the lines on a page have been retrieved, or
 * b) there are no more lines to retrieve from the file.
 */
 
void display_page()
{
	int number_of_chars;
	int number_of_lines;
	char line[LINE_WIDTH];
	int i;
	for( number_of_lines= 0; number_of_lines < PAGE_SIZE;
	 number_of_lines++ )
	{
		number_of_chars= fetch_next_line( line );
		if ( number_of_chars > 0 )
		{
			// print chars
			fwrite( line, sizeof(char), number_of_chars, stdout );
			// print '\n' if not included in line
			if ( line[number_of_chars-1] != '\n' )
				printf( "\n" );
		}
		else // EOF or error
		{
			if ( number_of_chars == 0 )
				printf( "=== EOF ===\n" );
			else
				printf( "(error reading file)\n" );
			return;
		}
	}
}

/* int fetch_next_line( char line[] )
 * Retrieves the next line of text from the buffer by
 * repeatedly calling fetch_next_word().
 * Each line breaks at either:
 * a) a LF ('\n') character, or
 * b) the last whitespace encountered at a position that is
 * <= (LINE_WIDTH + 1) (Why? Because if a line can contain 80
 * characters, but the last space between words occurs at
 * character 81, then the line can be broken at character 81.)
 * The line of text is stored in the line parameter.
 * Returns: the number of characters in the line, or -1 if
 * an error occurred.
 */

int fetch_next_line( char line[] )
{
	int num_of_chars;
	int i;

	for( i = 0; i <= LINE_WIDTH  + 1; i++ )
	{
		line = fetch_next_word( buffer, start_byte);
	}
	
	return 0;
}

/* int fetch_next_word( char word[], int max_size )
 * Reads bytes from the buffer until the next word is found.
 * Punctuation marks count as word characters; thus, the period
 * at the end of a sentence is included with the last word.
 * If the size of the word, plus trailing whitespace characters,
 * is <= (max_size + 1)
 *    copy the word to the buffer and return the number of chars
 * else
 *    return an error code
 * If the end of the buffer is reached while trying to determine
 * the next word, this function moves the remaining contents to
 * the beginning of the buffer and calls refill_buffer() to 
 * refill the remainder of the buffer.
 * Returns: the number of bytes in the word if successful;
 * otherwise, returns the following error values:
 *    0 if the EOF is reached;
 *   -1 if an error occurs; or
 *   -2 if the word length exceeds the max_size parameter.
 */
 
int fetch_next_word( char word[], int max_size )
{	
	int words_read;
	int number_of_chars;
	int bytes_in_buf;

	if (no_data = true)
		return 0;
	if ((last_byte - (start_byte + 1)) < max_size)
		refill_buffer(start_byte);
		if (no_data == true)
			return 0;
		
	bytes_in_buf = last_byte - (start_byte + 1);
	words_read = min( max_size, bytes_in_buf);

	if (words_read == 0)
		perror("---EOF---");
	if (words_read == -1)
		perror("error");	
	if (words_read == -2)
		perror("max size");

	start_byte += words_read;
	return words_read;
}



/* int refill_buffer( int start )
 * Refills the buffer, starting at the buffer index designated by the
 * start parameter, by reading bytes from the file.
 * Returns: the number of bytes read if successful; otherwise,
 * returns the error value returned from the call to read().
 */
 
int refill_buffer( int start )
{
	bool no_data = false;
	int bytes_read, num_remaining_bytes;
	int start_byte = start;

	if (start_byte <= last_byte)
	{
		int i;
		num_remaining_bytes = (last_byte - (start_byte + 1));
		for (i = 1600; i>= num_remaining_bytes; i--)
		{
			buffer[i+1] = buffer[i];
		} 
	}
	else
		num_remaining_bytes = 0;

	bytes_read = read(fd, (buffer + num_remaining_bytes), (1600 - bytes_read));
	bytes_in_buf = num_remaining_bytes + bytes_read;
	if (bytes_in_buf > 0){
		start_byte = 0;
		last_byte = bytes_in_buf - 1;
	}
	else
		no_data = true;
	if (bytes_read == -1)
		perror("file read");
	return bytes_read;
}

int main( int argc, char * argv[] )
{	
	// get the first command line argument
	// open the file
	// wait for commands--
	//  f   forward (next page)
	//  q   quit
	
	if ( argc != 2 )
		return usage( argv[0] );
		
	printf( "Opening file %s...\n", argv[1] );
	fd = open( argv[1], O_RDONLY );
	if ( fd == -1 )
	{
		perror( "open() failed" );
		return 1;
	}
	
	refill_buffer( 0 );
	
	// set up the terminal to eliminate buffering for stdio
	eliminate_stdio_buffering();

	char command= 'f'; // triggers display of first page
	do
	{
		switch( command )
		{
		case 'f':
			display_page();
			break;
		case 'q':
			// nothing to do now, but we could modify the code
			break;
		default:
			break;
		}
		command= (char) getchar();
	} while ( command != 'q' );
	
	close( fd );
	
	restore_stdio_buffering();
}

void eliminate_stdio_buffering()
{
	tcgetattr( 0, &old );
	new= old;
	new.c_lflag&= ~ICANON; // disable canonical mode
	new.c_lflag&= ~ECHO; // disable input echo
	tcsetattr( 0, TCSANOW, &new );
}

void restore_stdio_buffering()
{
	tcsetattr( 0, TCSANOW, &old );
}

