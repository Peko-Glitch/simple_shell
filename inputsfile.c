#include "shell.h"

int status;

int line_num;

char *shell_name;

/**
 * input_path - handles the user input from the command line
 * @old_buffer: old buffer to be handled
 * @o_size: size of old buffer
 *
 * Return: the new, sanitized buffer
 */
char *input_path(char *old_buffer, size_t *o_size)
{
	char *new_buffer = malloc(*o_size * 3);
	char *o_ptr = old_buffer;
	char *n_ptr = new_buffer;

	while (*o_ptr != '\0')
	{
		while (*o_ptr == ' ')
			o_ptr++;
		while (*o_ptr	!= ' ' && *o_ptr != ';' && *o_ptr != '|'
		       && *o_ptr != '&' && *o_ptr != '\0')
		{
			*n_ptr = *o_ptr;
			n_ptr++;
			o_ptr++;
		}
		while (*o_ptr == ' ')
			o_ptr++;
		if (n_ptr != new_buffer && *o_ptr != '\0')
		{
			*n_ptr = ' ';
			n_ptr++;
		}

		if (*o_ptr == ';' || *o_ptr == '|' || *o_ptr == '&')
		{
			if (err_check(o_ptr) == FALSE)
			{
				*o_size = 0;
				break;
			}
			*n_ptr = *o_ptr;
			if (*o_ptr == ';')
			{
				n_ptr++;
				*n_ptr = ' ';
			}
			else if (*(o_ptr + 1) == *o_ptr)
			{
				if (n_ptr == new_buffer)
				{
					message_err(o_ptr, NULL);
					*o_size = 0;
					break;
				}
				n_ptr++;
				*n_ptr = *o_ptr;
				n_ptr++;
				*n_ptr = ' ';
				o_ptr++;
			}
			n_ptr++;
			o_ptr++;
		}
	}
	*n_ptr = '\0';
	free(old_buffer);
	return (new_buffer);
}

/**
 * err_check - function that helps for sanitizing, of unexpected char
 * @ptr: pointer to area that needs to be checked
 *
 * Return: TRUE if no error detected, FALSE if there is an error
 */
int err_check(char *ptr)
{
	char *r = ptr;

	r++;
	if (*ptr == ';' && *r == *ptr)
	{
		message_err(ptr, NULL);
		return (FALSE);
	}
	if (*r == *ptr)
		r++;

	while (*r == ' ')
		r++;

	if (*r == ';' || *r == '|' || *r == '&')
	{
		message_err(r, NULL);
		return (FALSE);
	}

	return (TRUE);
}

/**
 * message_err - functions that prints an error messages and sets status
 * @arg0: command that is causing error
 * @arg1: first argument to command
 */
void message_err(char *arg0, char *arg1)
{
	char *err_str_num = _itoa(line_num);

	write(STDERR_FILENO, shell_name, _strlen(shell_name));
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, err_str_num, _strlen(err_str_num));
	free(err_str_num);

	if (str_cmp("cd", arg0, MATCH) == TRUE)
	{
		status = 2;
		write(STDERR_FILENO, ": cd: can't cd to ", 18);
		write(STDERR_FILENO, arg1, _strlen(arg1));
		write(STDERR_FILENO, "\n", 1);
		return;
	}

	if (str_cmp("exit", arg0, MATCH) == TRUE)
	{
		write(STDERR_FILENO, ": exit: Illegal number: ", 24);
		write(STDERR_FILENO, arg1, _strlen(arg1));
		write(STDERR_FILENO, "\n", 1);
		return;
	}
	if (*arg0 == ';' || *arg0 == '|' || *arg0 == '&')
	{
		status = 2;
		write(STDERR_FILENO, ": Syntax error: \"", 17);
		write(STDERR_FILENO, arg0, 1);
		if (*arg0 == *(arg0 + 1))
			write(STDERR_FILENO, arg0, 1);
		write(STDERR_FILENO, "\" unexpected\n", 14);
		return;
	}

	status = 127;
	write(STDERR_FILENO, ": ", 2);
	write(STDERR_FILENO, arg0, _strlen(arg0));
	write(STDERR_FILENO, ": not found\n", 12);
}

/**
 * check_vars - checks an argument to see if it is a variable and if so,
 * replaces it with it's value
 * @arg: argument to be checked
 *
 * Return: a pointer to the evaluated argument (either new or an altered clone)
 */
char *check_vars(char *arg)
{
	char *clon = NULL;
	int variable;
	char *p = arg;
	char *nxt;
	char *tmp;
	char *buf;
	int i;

	while (*p != '\0')
	{
		if (*p == '$')
		{
			if (clon == NULL)
			{
				clon = _strdup(arg);
				i = p - arg;
				p = clon + i;
			}
			nxt = p + 1;
			while (*nxt != '\0' && *nxt != '$' && *nxt != '#')
				nxt++;

			if (*nxt == '$' && nxt > p + 1)
				variable = TRUE;
			else if (*nxt == '#')
				variable = NEITHER;
			else
				variable = FALSE;

			*nxt = '\0';

			if (str_cmp("$?", p, MATCH) == TRUE)
				tmp = _itoa(status);
			else if (str_cmp("$0", p, MATCH) == TRUE)
				tmp = _strdup(shell_name);
			else if (get_array_element(environ, p + 1) != NULL)
			{
				buf = str_concat(p + 1, "=");
				tmp = _strdup(get_array_element
				(environ, buf) + _strlen(buf));
				free(buf);
			}
			else
				tmp = _strdup("");

			*p = '\0';
			p = str_concat(clon, tmp);
			free(tmp);
			if (variable == FALSE)
			{
				free(clon);
				clon = p;
				break;
			}
			if (variable == TRUE)
				*nxt = '$';
			else if (variable == NEITHER)
				*nxt = '#';
			tmp = str_concat(p, nxt);
			free(p);
			p = tmp;
			free(clon);
			clon = p;
			if (variable == NEITHER)
			{
				while (*p != '#')
					p++;
			}
		}
		p++;
	}
	if (clon != NULL)
		return (clon);

	return (arg);
}
