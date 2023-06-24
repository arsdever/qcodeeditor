#include "theme.hpp"

int main(int argc, char** argv)
{
    syntaxer::theme_ptr theme = syntaxer::parse_theme("test.theme");

    return 0;
}
