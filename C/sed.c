#include "types.h"
#include "stat.h"
#include "user.h"


char buf[1024];
int match(char*, char*);

void
sed(char *pattern, int fd)
{
	int n, m,w;
	char *p, *q;

	w = m = 0;
	while ((n = read(fd, buf + m, sizeof(buf) - m)) > 0) {
		m += n;
		p = buf;
		while ((q = strchr(p, '\n')) != 0) {
			*q = 0;
			if (match(pattern, p)) {
				*q = '\n';
				w++;
				//write(1, p, q + 1 - p);
			}
			p = q + 1;
		}
		printf(1, "%d\n", w);
		if (p == buf)
			m = 0;
		if (m > 0) {
			m -= p - buf;
			memmove(buf, p, m);
		}
	}
	printf(1, "%d\n", w);
}

int
main(int argc, char *argv[])
{
	int fd, i;
	char *pattern;

	if (argc <= 1) {
		sed("the", 0);
		exit();
	}
	pattern = argv[1];


	for (i = 1; i < argc; i++) {
		if ((fd = open(argv[i], 0)) < 0) {
			printf(1, "sed: cannot open %s\n", argv[i]);
			exit();
		}
		if (argc <= 2) {
			sed("the", fd);
			exit();
		}
		sed(pattern,fd);
		close(fd);
	}
	exit();
}

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9.

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
	if (re[0] == '^')
		return matchhere(re + 1, text);
	do {  // must look at empty string
		if (matchhere(re, text))
			return 1;
	} while (*text++ != '\0');
	return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
	if (re[0] == '\0')
		return 1;
	if (re[1] == '*')
		return matchstar(re[0], re + 2, text);
	if (re[0] == '$' && re[1] == '\0')
		return *text == '\0';
	if (*text != '\0' && (re[0] == '.' || re[0] == *text))
		return matchhere(re + 1, text + 1);
	return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
	do {  // a * matches zero or more instances
		if (matchhere(re, text))
			return 1;
	} while (*text != '\0' && (*text++ == c || c == '.'));
	return 0;
}


/*

void
sed(int fd, char *name, char *pattern)
{
int i, n;
int l, w, c, inword;

l = w = c = 0;
inword = 0;
while ((n = read(fd, buf, sizeof(buf))) > 0) {
for (i = 0; i<n; i++) {
c++;
if (buf[i] == '\n')
l++;
if (strchr(" \r\t\n\v", buf[i]))
inword = 0;
else if (!inword) {
inword = 1;
if (buf[i] == ) {

}
}
}
}
if (n < 0) {
printf(1, "sed: read error\n");
exit();
}
printf(1, "%d", w);
}



int
main(int argc, char *argv[])
{
int fd, i;
char *pattern;

if (argc <= 1) {
printf(2, "usage: grep pattern [file ...]\n");
exit();
}
pattern = argv[1];

if (argc <= 2) {
grep(pattern, 0);
exit();
}

for (i = 2; i < argc; i++) {
if ((fd = open(argv[i], 0)) < 0) {
printf(1, "grep: cannot open %s\n", argv[i]);
exit();
}
grep(pattern, fd);
close(fd);
}
exit();
}

char line[];
while (*p != "\n") {
char search[] = p[0]
if ( == "the") {
line += *pattern2;
}
else {
line += *p;
}
p++;
}

/*
char buf[1024];
int match(char*, char*);

void
sed(char *pattern, char* pattern2, int fd)
{
int n,m,w;
char *p, *q;

w = m = 0;
while ((n = read(fd, buf + m, sizeof(buf) - m)) > 0) {
m += n;
p = buf;
while ((q = strchr(p, '\n')) != 0) {
*q = 0;
if (match(pattern, p)) {
*q = '\n';
w++;
}
p = q + 1;
}

if (p == buf)
m = 0;
if (m > 0) {
m -= p - buf;
memmove(buf, p, m);
}
}

if (n < 0) {
printf(1, "sed: read error\n");
exit();
}
printf(1, "%d\n", w);
}

int
main(int argc, char *argv[])
{
int fd, i;

if (argc <= 1) {
sed("the", 0);
exit();
}

for (i = 1; i < argc; i++) {
if ((fd = open(argv[i], 0)) < 0) {
printf(1, "sed: cannot open %s\n", argv[i]);
exit();
}
if (argc <= 2){
sed("the", fd);
}
else if (argc <= 4) {
sed(arg[1], fd);
}
close(fd);
}
exit();
}

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9.

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
if (re[0] == '^')
return matchhere(re + 1, text);
do {  // must look at empty string
if (matchhere(re, text))
return 1;
} while (*text++ != '\0');
return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
if (re[0] == '\0')
return 1;
if (re[1] == '*')
return matchstar(re[0], re + 2, text);
if (re[0] == '$' && re[1] == '\0')
return *text == '\0';
if (*text != '\0' && (re[0] == '.' || re[0] == *text))
return matchhere(re + 1, text + 1);
return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
do {  // a * matches zero or more instances
if (matchhere(re, text))
return 1;
} while (*text != '\0' && (*text++ == c || c == '.'));
return 0;
}

*/

#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"


char buf[512];


/*
void sed(char* search, char* replace, int fd) {
int i, n, l, x;
int pos1, pos2, wc, exists, counter, count = 0;
char* lines;

lines = malloc(512 * sizeof(char));
//printf(1, "search : %s", search);

while ((n = read(fd, buf, sizeof(buf))) > 0) {
pos1 = 0;
for (i = 0; i < n; ++i) {
counter++;
//Part 3: Replacing "the" with "xyz"
if (buf[i] == search[0] && search[0] != '-') {
if (buf[i + 1] == search[1] && buf[i + 2] == search[2]) {
buf[i] = replace[0];
buf[i + 1] = replace[1];
buf[i + 2] = replace[2];
wc++; //Part 2: Counting the occurrences of the word
exists = 1;
}
}
//Part 4: Replacing the word with given word
if (search[0] == '-' && buf[i] == search[1]) {
for (x = 0; x < strlen(search); ++x) {
if (buf[i + x] == search[x + 1]) {
++count;
}
}
}
if (count == strlen(search) - 1) {
for (x = 0; x < strlen(replace); ++x) {
buf[i + x] = replace[x + 1];
}
wc++;
exists = 1;
}
count = 0;


lines[i] = buf[i];
//Part 3: Printing the changed lines
if (buf[i] == '\n' && exists == 1) {
pos2 = i;
exists = 0;
for (l = pos1; l < pos2+1; ++l) {
printf(1, "%c", lines[l]);
}
}
if (counter > 512) {
char* temp = malloc(2 * sizeof(lines));
memmove(temp, lines, sizeof(lines));
free(lines);
lines = temp;

}
if (buf[i] == '\n') {
pos1 = i + 1;
free(lines);
lines = malloc(512 * sizeof(char));
counter = 0;
}

}
//Part 3: Printing the changed lines
if (pos1 > pos2 && exists == 1) {
for (l = pos1; l < n; ++l) {
printf(1, "%c", lines[l]);
}
printf(1, "\n");
exists = 0;
pos1 = pos2 = 0;
free(lines);
lines = malloc(512 * sizeof(char));
counter = 0;
}
}
free(lines);
close(fd);
if (n < 0) {
printf(1, "sed: read error\n");
exit();
}
//Part 2: Printing the occurrences of the word
printf(1, "Found and replaced %d occurrences\n", wc);

}

*/

void sed(char* search, char* replace, int fd) {
	int i, n, l, counter, x;
	int pos1, pos2, wc, exists, count = 0;

	while ((n = read(fd, buf, sizeof(buf))) > 0) {
		pos1 = counter = 0;
		for (i = 0; i < n; ++i) {
			//Part 4: Replacing the word with given word
			if (buf[i] == search[0] && search[0] != '-') {
				if (buf[i + 1] == search[1] && buf[i + 2] == search[2]) {
					buf[i] = replace[0];
					buf[i + 1] = replace[1];
					buf[i + 2] = replace[2];
					wc++; //Part 2: Counting the occurrences of the word
					exists = 1;
				}
			}
			//Part 4: Replacing the word with given word
			if (search[0] == '-' && buf[i] == search[1]) {
				for (x = 0; x < strlen(search); ++x) {
					if (buf[i + x] == search[x + 1]) {
						++count;
					}
				}
			}
			if (count == strlen(search) - 1) {
				for (x = 0; x < strlen(replace); ++x) {
					buf[i + x] = replace[x + 1];
				}
				wc++;
				exists = 1;
			}
			count = 0;

			//Part 3: Printing the changed lines
			if (buf[i] == '\n' && exists == 1) {
				pos2 = i;
				exists = 0;
				for (l = pos1; l < pos2 + 1; ++l) {
					printf(1, "%c", buf[l]);
				}
			}
			if (buf[i] == '\n') {
				pos1 = i + 1;
			}

		}
		//Part 3: Printing the changed lines
		if (pos1 > pos2 && exists == 1) {
			for (l = pos1; l < n; ++l) {
				printf(1, "%c", buf[l]);
			}

			printf(1, "\n");
			exists = 0;
			pos1 = pos2 = 0;
		}
	}
	close(fd);
	if (n < 0) {
		printf(1, "sed: read error\n");
		exit();
	}
	//Part 2: Printing the occurrences of the word
	printf(1, "Found and replaced %d occurrences\n", wc);

}


int main(int argc, char *argv[]) {
	int fd;
	//Given just the command, piping in standard input
	if (argc <= 1) {
		sed("the", "xyz", 0);
		exit();
	}
	//Given the filename
	if (argc <= 2) {
		if ((fd = open(argv[1], 0)) < 0) {
			printf(1, "sed: cannot open %s\n", argv[1]);
			exit();
		}
		sed("the", "xyz", fd);
	}
	//Given the filename, word to be searched for, and word to replace searched word
	else if (argc <= 4) {
		if ((fd = open(argv[3], 0)) < 0) {
			printf(1, "sed: cannot open %s\n", argv[3]);
			exit();
		}
		sed(argv[1], argv[2], fd);
	}
	else {
		printf(1, "sed: too many arguments\n");
		exit();
	}
	close(fd);
	exit();

}

/*if (buf[i] == '\n' && pos1 == 0 && exists == 1) {
pos2 = i;
exists = 0;
for (l = pos1; l < pos2; ++l) {
printf(1, "%c", buf[l]);
}
printf(1, "\n");
pos1 = 1;
}
*/

/*if (buf[i] == '\n' && pos1 == 0 && exists == 1) {
pos2 = i;
exists = 0;
for (l = pos1; l < pos2; ++l) {
printf(1, "%c", buf[l]);
}
printf(1, "\n");
pos1 = 1;
}
*/

*

void sed(char* search, char* replace, int fd) {
	int i, n, l, counter, x;
	int pos1, pos2, wc, exists, count = 0;

	while ((n = read(fd, buf, sizeof(buf))) > 0) {
		pos1 = counter = 0;
		for (i = 0; i < n; ++i) {
			//Part 4: Replacing the word with given word
			if (buf[i] == search[0] && search[0] != '-') {
				if (buf[i + 1] == search[1] && buf[i + 2] == search[2]) {
					buf[i] = replace[0];
					buf[i + 1] = replace[1];
					buf[i + 2] = replace[2];
					wc++; //Part 2: Counting the occurrences of the word
					exists = 1;
				}
			}
			//Part 4: Replacing the word with given word
			if (search[0] == '-' && buf[i] == search[1]) {
				for (x = 0; x < strlen(search); ++x) {
					if (buf[i + x] == search[x + 1]) {
						++count;
					}
				}
			}
			if (count == strlen(search) - 1) {
				for (x = 0; x < strlen(replace); ++x) {
					buf[i + x] = replace[x + 1];
				}
				wc++;
				exists = 1;
			}
			count = 0;

			//Part 3: Printing the changed lines
			if (buf[i] == '\n' && exists == 1) {
				pos2 = i;
				exists = 0;
				for (l = pos1; l < pos2 + 1; ++l) {
					printf(1, "%c", buf[l]);
				}
			}
			if (buf[i] == '\n') {
				pos1 = i + 1;
			}

		}
		//Part 3: Printing the changed lines
		if (pos1 > pos2 && exists == 1) {
			for (l = pos1; l < n; ++l) {
				printf(1, "%c", buf[l]);
			}

			printf(1, "\n");
			exists = 0;
			pos1 = pos2 = 0;
		}
	}
	close(fd);
	if (n < 0) {
		printf(1, "sed: read error\n");
		exit();
	}
	//Part 2: Printing the occurrences of the word
	printf(1, "Found and replaced %d occurrences\n", wc);

}
*/
