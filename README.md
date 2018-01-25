# GetHub
Small application utilizing QT framework and Github api's

### Compile/Install
+ Download/Clone GetHub repo to local machine.

*Enter Following Commands*
+ `cd to/download/site/GetHub`
+ `qmake GetHub.pro`
+ `make`
+ `./GetHub`

### Use
Type in a github username and hit the find button, displayed in the listwidget will be the user's repositories. 

If a repository is clicked on the entire repository tree will be fetched and displayed in the treewidget where it can be expanded and explored.  

When an item in the tree widget is double clicked the associated url will be opened up in a default browser. If the item is a file then when the item is clicked it will download the file.

All actions and results are noted through the info line at the bottom of application.

### Dependencies
  + nlohmann/json -- [https://github.com/nlohmann/json] -- (Included through json.hpp)
  + mrtazz/restclient-cpp -- [https://github.com/mrtazz/restclient-cpp]

### Contact
andres.imperial@live.com
