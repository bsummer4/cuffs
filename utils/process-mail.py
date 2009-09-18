#!/usr/bin/python

"""
This takes a email message (RFC5322), and produces a some text
suitable for inserting into an Unfuddle message box (markdown).

Performs the following transformations:

  - Remove everything from the header except the 'from' field
  - Add the subject as the primary header for the message.
  - Increase the indention for all other headers so that they are
    subheaders to the message subject

* TODO Might not work well with all markdown texts
  This works with the subset of markdown I (Benjamin Summers) use, so
  it there might be some valid markdown that's not handled correctly.
"""

def emptyp(sequence): return not len(sequence)
def p(x): print x; return x
def identity(x): return x


def alist_to_dict(alist):
    """ alist_to_dict({3: 5}.items()) == {3: 5}
        alist_to_dict([(3, 4), (3, 5)]) => {3: 4} """
    result = {}
    for key, value in reversed(alist):
        result[key] = value
    return result

"An ad-hoc mail library"

def read_header(stream):
    "Return a list of lines in the header.  "
    header = []
    while 1:
        line = stream.readline()
        if emptyp(line.strip()): break
        header.append(line)
    return header

def handle_fields(field, value): return field.lower(), value
def parse_header_line(line):
    sections = line.strip().split(":")
    return handle_fields(sections[0], ":".join(sections[1:]).strip())

def parse_header(lines):
    "Dictionary of fields in the header.  " 
    return alist_to_dict(filter(identity, map(parse_header_line, lines)))

def read_message(stream):
    "(header dictionary, body lines)"
    return (parse_header(read_header(stream)), stream.readlines())


"Modify the message.  "

def deepen_indents_destructive(markdown_lines):
    "For every line that starts a # has one more # added to the front."
    for index in range(len(markdown_lines)):
        line = markdown_lines[index]
        first_char = line[0]
        if first_char in ["#"]:
            markdown_lines[index] = first_char + line
    return markdown_lines

def render_header(header):
    """Take what we want from the header.  Keeps the from field and
       turns the subject into a header.  '(no subject)' is used if
       that field is not specified.  """
    result = ""
    if "from" in header: result += "from: " + header['from'] + "\n\n"
    if "subject" in header: result += "# %s\n"%header['subject']
    else: result += "# %s\n"%"(no subject)"
    return result

def niceify(header, body):
    return render_header(header) + ''.join(deepen_indents_destructive(body))

def process_mail(stream): return apply(niceify, read_message(sys.stdin))

if __name__ == '__main__':
    import sys, pprint
    sys.stdout.write(process_mail(sys.stdin))
