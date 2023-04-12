CXX = g++
CXXFLAGS += -I/opt/homebrew/include

PROJ = tic-tac-toe
#OBJS = main.o model.o view.o controller.o 
OBJS = main.o
LIBS = -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system
RM = rm

PROJ_2 = SFML_test
OBJS_2 = SFML_test.o

$(PROJ): $(OBJS)
	$(CXX) $(OBJS) -o $(PROJ) $(LIBS)

clean:
	$(RM) $(OBJS) $(PROJ)

$(PROJ_2): $(OBJS_2)
	$(CXX) $(OBJS_2) -o $(PROJ_2) $(LIBS)

-include.depence:

depend: 
	$(CXX) -MM $(OBJS:.o=.cc) > .depence


