#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

namespace syntaxer
{

class item_t;
class theme_t;
class color_t;
class font_t;

using uuid_t = std::string;
using theme_ptr = std::shared_ptr<theme_t>;
using color_ptr = std::shared_ptr<color_t>;
using font_ptr = std::shared_ptr<font_t>;

struct color_space_t {
    std::string displayP3_PQ;
    std::string extendedDisplayP3;
    std::string extendedITUR_2020;
    std::string itur_2020_PQ;
    std::string itur_2020_sRGBGamma;
    std::string itur_2100_HLG;
    std::string itur_2100_PQ;
    std::string itur_709_HLG;
    std::string itur_709_PQ;
    std::string linearDisplayP3;
    std::string linearITUR_2020;
};

using color_space_ptr = std::shared_ptr<color_space_t>;

struct styles_t {
    styles_t(
        color_ptr foreground,
        color_ptr background,
        color_ptr caret,
        color_ptr selection,
        font_ptr font,
        bool underlined,
        bool strikethrough,
        bool misspelled
    )
        : _foreground(foreground)
        , _background(background)
        , _caret(caret)
        , _selection(selection)
        , _font(font)
        , _underlined(underlined)
        , _strikethrough(strikethrough)
        , _misspelled(misspelled)
    {
    }

    styles_t() = default;
    color_ptr foreground() const { return _foreground; }
    color_ptr background() const { return _background; }
    color_ptr caret() const { return _caret; }
    color_ptr selection() const { return _selection; }
    font_ptr font() const { return _font; }
    bool underlined() const { return _underlined; }
    bool strikethrough() const { return _strikethrough; }
    bool misspelled() const { return _misspelled; }

private:
    color_ptr _foreground;
    color_ptr _background;
    color_ptr _caret;
    color_ptr _selection;
    font_ptr _font;
    bool _underlined;
    bool _strikethrough;
    bool _misspelled;
};

struct gutter_styles_t {
    gutter_styles_t() { }
    gutter_styles_t(gutter_styles_t const& rhs) = delete;
    gutter_styles_t& operator=(gutter_styles_t const& rhs) = delete;
    ~gutter_styles_t();

    bool is_transparent() const;
    void clear();

    color_ptr divider = nullptr;
    color_ptr selection_border = nullptr;

    color_ptr foreground = nullptr;
    color_ptr background = nullptr;
    color_ptr icons = nullptr;
    color_ptr icons_hover = nullptr;
    color_ptr icons_pressed = nullptr;

    color_ptr selection_foreground = nullptr;
    color_ptr selection_background = nullptr;
    color_ptr selection_icons = nullptr;
    color_ptr selection_icons_hover = nullptr;
    color_ptr selection_icons_pressed = nullptr;
};

class theme_t
{
public:
    theme_t(
        const nlohmann::json& theme_json,
        std::string_view font_name = "",
        float font_size = 12
    );

    theme_ptr copy_with_font_name_and_size(
        std::string const& font_name, float font_size
    );

    uuid_t const& uuid() const;
    std::string font_name() const;
    float font_size() const;
    color_ptr foreground() const;
    color_ptr background(std::string const& file_type = "") const;
    bool is_dark() const;
    bool is_transparent() const;
    gutter_styles_t const& gutter_styles() const;
    styles_t const& styles_for_scope(std::string_view scope) const;

    struct color_info_t {
        color_info_t()
            : red(-1)
            , green(0)
            , blue(0)
            , alpha(1)
        {
        }
        color_info_t(double red, double green, double blue, double alpha = 1)
            : red(red)
            , green(green)
            , blue(blue)
            , alpha(alpha)
        {
        }

        bool is_blank() const { return red < 0; }
        bool is_opaque() const { return alpha == 1; };

        double red, green, blue, alpha;
    };

private:
    enum bool_t { bool_true, bool_false, bool_unset };

    struct decomposed_style_t {
        decomposed_style_t(
            std::string_view scope_selector = "",
            std::string_view font_name = "",
            float font_size = -1
        )
            : scope_selector(scope_selector)
            , font_name(font_name)
            , font_size(font_size)
            , bold(bool_unset)
            , italic(bool_unset)
            , underlined(bool_unset)
            , strikethrough(bool_unset)
            , misspelled(bool_unset)
        {
        }

        decomposed_style_t& operator+=(decomposed_style_t const& rhs);

        std::string scope_selector;

        std::string font_name;
        float font_size;
        color_info_t foreground;
        color_info_t background;
        color_info_t caret;
        color_info_t selection;
        color_info_t invisibles;
        bool_t bold;
        bool_t italic;
        bool_t underlined;
        bool_t strikethrough;
        bool_t misspelled;
    };

    static std::vector<decomposed_style_t> global_styles(std::string_view scope
    );

    struct shared_styles_t {
        shared_styles_t(const nlohmann::json& item);
        ~shared_styles_t();
        void setup_styles();
        static decomposed_style_t parse_styles(const nlohmann::json& json);

        nlohmann::json _item;
        color_space_ptr _color_space = nullptr;
        std::vector<decomposed_style_t> _styles;
        gutter_styles_t _gutter_styles;
        color_ptr _foreground = nullptr;
        color_ptr _background = nullptr;
        bool _is_dark;
        bool _is_transparent;
    };

    using shared_styles_ptr = std::shared_ptr<shared_styles_t>;
    shared_styles_ptr find_shared_styles(const nlohmann::json& theme_json);

private:
    nlohmann::json _json;
    shared_styles_ptr _styles;
    std::string _font_name;
    float _font_size;
    mutable std::unordered_map<std::string, styles_t> _cache;
};

theme_ptr parse_theme(const nlohmann::json& theme_path);

} // namespace syntaxer
