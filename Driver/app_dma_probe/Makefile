sources=$(wildcard *.c)
objs=$(sources:.c=.o)

all: app 
app: $(objs)
	@echo "Building output binary.."
	$(CC) -g  -o $@ $(objs)
%.o: %.c
	@echo "Compiling sources.."
	$(CC) -g -c $(sources)
%.d: %.c
	@echo -n  "Creating dependancies.."
	@echo $@
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,    \($*\)\.o[ :]*     ,    \1.o $@ :   ,   g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
clean:
	@rm -f app *.o *.d*
	@echo "Clean done.."

-include $(sources:.c=.d)


