# Cloud detection in optical satellite images
## Introduction

This paper is an implementation of [Introducing spatial information in k-means algorithm for cloud detection in optical satellite images](http://adsabs.harvard.edu/abs/2001SPIE.4168...67B).

Here below, some examples:

The output corresponds to the percentage of clouds in images

![Example 1](res/first_result.png?raw=true)

![Example 2](res/second_result.png?raw=true)

Output : 
```Shell
30312549607241130511x.bmp => 21.949615%
36683199502211443252x.bmp => 0.981154%
```

## Installation

+ Clone the repository

```Shell
git clone https://github.com/zined1/cloud_detection.git
cd cloud_detection 
mkdir obj
```
+ Build
```Shell
cd src
make
```
+ Run 
```Shell
./imaProjet.exe
```
