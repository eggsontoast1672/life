build:
  cmake --build build/

run: build
  build/gameoflife

clean:
  rm -fr build/

distclean: clean
  rm -fr .cache/
