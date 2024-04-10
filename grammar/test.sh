grammar < raw.leg

meson compile thing -C ../builddir
meson compile thing2 -C ../builddir

diff ../builddir/grammar/thing ../builddir/grammar/thing2

