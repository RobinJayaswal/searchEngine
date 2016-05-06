## **README for common.a library**
## Robin Jayaswal, Kyle Dotterrer, May 2016

### **Overview**
The _common_ library (_common.a_) provides four separate modules that generalize
multiple functionalities required by TSE. The library includes the modules _web_, 
_word_, _index_, and _file_.

### **Library Compilation**
The _common_ library (_common.a_) is built completely by a single invocation of
the _make_ command from within the common/ directory. The library is linked in
to TSE programs during the linking phase of each respective program's 
compilation. 

### **Module Descriptions**
#### _web_
* The web module (courtesy of David Kotz) provides functions for fetching and 
parsing URLs and their corresponding webpages. The module's exported functions
and their descriptions are located in _web.h_, while the module's implementation
is in _web.c_. 

#### _word_
* The word module (courtesy of David Kotz) provides functions for normalizing and
fectching words from a string. The module's exported functions and their 
descriptions are located in _word.h_, while the module's implementation is in
_word.c_. 

#### _index_
* The index module provides functions for loading the file representation
of an inverted index into its corresponding data structure and saving such a 
data structure to its file representation. The module's exported functions and 
their descriptions are located in _index.h_, while the module's implementation 
is in _index.c_. 

#### _file_
* The file module (courtesy of David Kotz) provides functions that aid in the 
controlled reading of text files. The module's exported functions and 
their descriptions are located in _file.h_, while the module's implementation 
is in _file.c_. 


