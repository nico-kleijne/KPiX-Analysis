# KPiX-Analysis

Desciption: analysis code for R&D use on the LYCORIS telescope project.

### Prerequisite:
--------------

In order to run this package, you will need:
* libkpix.so produced from the kpix DAQ: https://github.com/Lycoris2017/KPiX-Lycoris.git
* you need to either configure the LD_LIBRARY_PATH to have the 'make install'ed kpix package to be used for this package; or you can
```
mkdir lib
cp $KPIX-DAQ/libkpix.so lib/
```

### Basic setup:
--------------

#### Add the central KPiX-Daq repository to get the master branch:

```
git clone https://github.com/Lycoris2017/KPiX-Analysis.git kpix_ana
cd kpix_ana
```

#### (Option) As developper:

If you are a developper of this software, please click the 'Fork' on top right of this page. Then add your mirror and push your 'dev' branch to it:

```
git remote add myana git@github.com:$YOUR_GITHUB_REPOSITORY/KPiX-Analysis.git
git checkout -b local.dev
git commit -m "init commit, copy from central master branch"
git push -u myana local.dev
```

Collaboration ATTENTION! Please always push to your own branch, then to ask for a pull-request on the central git repository!

#### Compile

```
make -j 4
```
Make options:
* mini: only analysis.cxx compiled

#### Branch descriptions:

* master: up-to-date latest development version.


