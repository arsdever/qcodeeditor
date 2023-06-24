#include <filesystem>
#include <map>

#include "theme.hpp"

namespace syntaxer
{

theme_t::theme_t(
    const nlohmann::json& theme_json,
    std::string_view font_name,
    float font_size
)
    : _json(theme_json)
    , _font_name(font_name)
    , _font_size(font_size)
{
    _styles = find_shared_styles(theme_json);
}

theme_ptr parse_theme(const nlohmann::json& theme_json)
{
    static std::map<uuid_t, theme_ptr> cache;

    uuid_t const& uuid = theme_json[ "uuid" ];
    auto theme = cache.find(uuid);
    if (theme == cache.end())
        theme =
            cache.emplace(uuid, std::make_shared<theme_t>(theme_json)).first;

    return theme->second;
}

theme_t::shared_styles_t::shared_styles_t(const nlohmann::json& item)
    : _item(item)
{
}

void theme_t::shared_styles_t::setup_styles()
{
    _styles.clear();
    _gutter_styles.clear();

    if (_color_space)
        _color_space = nullptr;

    if (_item) {
        if (const nlohmann::json& newItem = _item[ "uuid" ])
            _item = newItem;

        std::string colorSpaceName;
        if (plist::get_key_path(
                _item->plist(), "colorSpaceName", colorSpaceName
            ) &&
            colorSpaceName == "sRGB") {
            if (_color_space)
                CGColorSpaceRelease(_color_space);
            _color_space = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
        }

        plist::array_t items;
        if (plist::get_key_path(_item->plist(), "settings", items)) {
            for (auto const& it : items) {
                if (plist::dictionary_t const* styles =
                        boost::get<plist::dictionary_t>(&it)) {
                    _styles.push_back(parse_styles(*styles));
                    if (!_styles.back().invisibles.is_blank()) {
                        decomposed_style_t invisibleStyle("deco.invisible");
                        invisibleStyle.foreground = _styles.back().invisibles;
                        _styles.push_back(invisibleStyle);
                    }
                }
            }
        }
    }

    if (!_color_space)
        _color_space = CGColorSpaceCreateWithName(kCGColorSpaceGenericRGB);

    // =======================================
    // = Find “global” foreground/background =
    // =======================================

    // We assume that the first style is the unscoped root style

    _foreground =
        (_styles.empty() ? CGColorPtr()
                         : OakColorCreateFromThemeColor(
                               _styles[ 0 ].foreground, _color_space
                           ))
            ?: CGColorPtr(
                   CGColorCreate(_color_space, (CGFloat[ 4 ]) { 1, 1, 1, 1 }),
                   CGColorRelease
               );
    _background =
        (_styles.empty() ? CGColorPtr()
                         : OakColorCreateFromThemeColor(
                               _styles[ 0 ].background, _color_space
                           ))
            ?: CGColorPtr(
                   CGColorCreate(_color_space, (CGFloat[ 4 ]) { 0, 0, 0, 1 }),
                   CGColorRelease
               );
    _is_dark = color_is_dark(_background.get());
    _is_transparent = CGColorGetAlpha(_background.get()) < 1;

    // =========================
    // = Default Gutter Styles =
    // =========================

    _gutter_styles.divider = OakColorCreateCopySoften(_foreground, 0.4);
    _gutter_styles.foreground = OakColorCreateCopySoften(_foreground, 0.5);
    _gutter_styles.background = OakColorCreateCopySoften(_background, 0.87);
    _gutter_styles.selectionForeground =
        OakColorCreateCopySoften(_foreground, 0.95);
    _gutter_styles.selectionBackground =
        OakColorCreateCopySoften(_background, 0.95);

    plist::dictionary_t gutterSettings;
    if (_item &&
        plist::get_key_path(_item->plist(), "gutterSettings", gutterSettings)) {
        static struct {
            std::string const key;
            CGColorRef gutter_styles_t::*field;
        } const gutterKeys[] = {
            {              "divider",&gutter_styles_t::divider                                     },
            {      "selectionBorder",     &gutter_styles_t::selectionBorder},
            {           "foreground",          &gutter_styles_t::foreground},
            {           "background",          &gutter_styles_t::background},
            {                "icons",               &gutter_styles_t::icons},
            {           "iconsHover",          &gutter_styles_t::iconsHover},
            {         "iconsPressed",        &gutter_styles_t::iconsPressed},
            {  "selectionForeground", &gutter_styles_t::selectionForeground},
            {  "selectionBackground", &gutter_styles_t::selectionBackground},
            {       "selectionIcons",      &gutter_styles_t::selectionIcons},
            {  "selectionIconsHover", &gutter_styles_t::selectionIconsHover},
            {"selectionIconsPressed",
             &gutter_styles_t::selectionIconsPressed                       },
        };

        for (auto const& gutterKey : gutterKeys) {
            theme_t::color_info_t color;
            if (get_key_path(gutterSettings, gutterKey.key, color)) {
                if (_gutter_styles.*(gutterKey.field))
                    CGColorRelease(_gutter_styles.*(gutterKey.field));
                _gutter_styles.*(gutterKey.field) = CGColorRetain(
                    OakColorCreateFromThemeColor(color, _color_space).get()
                );
            }
        }
    }

    _gutter_styles.selectionBorder =
        _gutter_styles.selectionBorder ?: CGColorRetain(_gutter_styles.divider);
    _gutter_styles.icons =
        _gutter_styles.icons ?: CGColorRetain(_gutter_styles.foreground);
    _gutter_styles.iconsHover =
        _gutter_styles.iconsHover ?: CGColorRetain(_gutter_styles.icons);
    _gutter_styles.iconsPressed =
        _gutter_styles.iconsPressed ?: CGColorRetain(_gutter_styles.icons);
    _gutter_styles.selectionIcons =
        _gutter_styles.selectionIcons
            ?: CGColorRetain(_gutter_styles.selectionForeground);
    _gutter_styles.selectionIconsHover =
        _gutter_styles.selectionIconsHover
            ?: CGColorRetain(_gutter_styles.selectionIcons);
    _gutter_styles.selectionIconsPressed =
        _gutter_styles.selectionIconsPressed
            ?: CGColorRetain(_gutter_styles.selectionIcons);
}

} // namespace syntaxer
