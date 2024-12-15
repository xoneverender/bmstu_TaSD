# Компилятор и общие флаги
CC = gcc
SRC_DIR = src
INC_DIR = inc
OUT_DIR = out
TEST_DIR = unit_tests
CFLAGS = -std=c99 -Wall -Wno-unused-result -Wpedantic -Wextra -Wvla -I$(INC_DIR)
LDFLAGS = -lm
EXEC = app.exe

# Флаги для сборок
DEBUG_FLAG = $(OUT_DIR)/debug_flag
RELEASE_FLAG = $(OUT_DIR)/release_flag
ASAN_FLAG = $(OUT_DIR)/asan_flag
MSAN_FLAG = $(OUT_DIR)/msan_flag
UBSAN_FLAG = $(OUT_DIR)/ubsan_flag

# Исходники, объектные файлы и файлы зависимостей
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OUT_DIR)/%.o, $(SRC))
DEPS = $(OBJS:.o=.d)

# Если не указана цель - по умолчанию создается релизная сборка
all: release

# Дебаг-сборка
debug: $(DEBUG_FLAG)
debug: CFLAGS += -O0 -g
debug: $(EXEC)

# Релиз-сборка
release: $(RELEASE_FLAG)
release: CFLAGS += -O2
release: $(EXEC)

# AddressSanitizer (использует clang)
debug_asan: CC = clang
debug_asan: $(ASAN_FLAG)
debug_asan: CFLAGS += -fsanitize=address -fno-omit-frame-pointer -O0 -g
debug_asan: LDFLAGS += -fsanitize=address
debug_asan: $(EXEC)

# MemorySanitizer (использует clang)
debug_msan: CC = clang
debug_msan: $(MSAN_FLAG)
debug_msan: CFLAGS += -fsanitize=memory -fPIE -fno-omit-frame-pointer -O0 -g
debug_msan: LDFLAGS += -fsanitize=memory -pie
debug_msan: $(EXEC)

# Undefined Behavior Sanitizer (использует clang)
debug_ubsan: CC = clang
debug_ubsan: $(UBSAN_FLAG)
debug_ubsan: CFLAGS += -fsanitize=undefined -fno-omit-frame-pointer -O0 -g
debug_ubsan: LDFLAGS += -fsanitize=undefined
debug_ubsan: $(EXEC)

# Создание новых флагов и удаление предыдущих
%_flag:
	rm -f $(OUT_DIR)/*_flag $(OUT_DIR)/*.o
	touch $@

# Создание файлов зависимостей для основных файлов
$(OUT_DIR)/%.d: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MM -MT $(OUT_DIR)/$*.o $< > $@

# Компиляция тестов
$(OUT_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	
# Компиляция
$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Компоновка
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Удаление генерируемых файлов
clean:
	rm -f $(OUT_DIR)/*.o $(OUT_DIR)/*.gcov $(OUT_DIR)/*.gcda $(OUT_DIR)/*.gcno $(OUT_DIR)/*_flag $(OUT_DIR)/*.d $(EXEC)

# Включение файлов зависимостей
include $(DEPS)

# Обозначение всех целей как фиктивных
.PHONY: all release debug debug_asan debug_msan debug_ubsan clean