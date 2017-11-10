#!/bin/bash

gcc -lm $1.c dm.c -o dm
echo "Projet compilé avec $1"
