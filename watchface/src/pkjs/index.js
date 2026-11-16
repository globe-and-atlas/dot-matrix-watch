var Clay = require('@rebble/clay');
var defaults = {Palette: 0, Background: 0, Foreground: 0x55FF55, Accent: 0x55FF55, TimeFormat: 0, DateFormat: 1};

function integer(value, fallback, maximum) {
  if (value === undefined || value === null || value === '' || typeof value === 'boolean') return fallback;
  var number = Number(value);
  return isFinite(number) && Math.floor(number) === number && number >= 0 && number <= maximum ? number : fallback;
}
function normalize(raw) {
  raw = raw || {};
  return {
    Palette: integer(raw.Palette, defaults.Palette, 6),
    Background: integer(raw.Background, defaults.Background, 0xFFFFFF),
    Foreground: integer(raw.Foreground, defaults.Foreground, 0xFFFFFF),
    Accent: integer(raw.Accent, defaults.Accent, 0xFFFFFF),
    TimeFormat: integer(raw.TimeFormat, defaults.TimeFormat, 2),
    DateFormat: integer(raw.DateFormat, defaults.DateFormat, 4)
  };
}
var clay = new Clay([
  {type: 'heading', defaultValue: 'Dot Matrix'},
  {type: 'text', defaultValue: 'Original dot numerals, with a compact date and dotted battery display.'},
  {type: 'select', messageKey: 'TimeFormat', label: 'Time format', defaultValue: '0', options: [
    {label: 'Watch default', value: '0'}, {label: '12-hour', value: '1'}, {label: '24-hour', value: '2'}
  ]},
  {type: 'select', messageKey: 'DateFormat', label: 'Date', defaultValue: '1', options: [
    {label: 'Off', value: '0'}, {label: 'THU NOV 05', value: '1'},
    {label: 'YYYY-MM-DD', value: '2'}, {label: 'MM/DD/YYYY', value: '3'}, {label: 'DD/MM/YYYY', value: '4'}
  ]},
  {type: 'heading', defaultValue: 'Colors'},
  {type: 'select', messageKey: 'Palette', label: 'Palette', defaultValue: '0', options: [
    {label: 'Mint', value: '0'}, {label: 'Amber', value: '1'},
    {label: 'Ice', value: '2'}, {label: 'Rose', value: '3'}, {label: 'White', value: '4'},
    {label: 'Paper', value: '5'}, {label: 'Custom colors', value: '6'}
  ]},
  {type: 'text', defaultValue: 'Select Custom colors to use the choices below. The accent colors the dotted divider and battery display. Choose contrasting text and background colors.'},
  {type: 'color', messageKey: 'Background', label: 'Background', defaultValue: '000000', sunlight: false, allowGray: true},
  {type: 'color', messageKey: 'Foreground', label: 'Digits & labels', defaultValue: '55FF55', sunlight: false, allowGray: true},
  {type: 'color', messageKey: 'Accent', label: 'Accent', defaultValue: '55FF55', sunlight: false, allowGray: true},
  {type: 'submit', defaultValue: 'Save to watch'}
], null, {autoHandleEvents: false});

function sendSaved() {
  var raw = localStorage.getItem('clay-settings');
  if (!raw) return;
  try { Pebble.sendAppMessage(normalize(JSON.parse(raw))); } catch (error) { console.log('Dot Matrix settings could not be loaded'); }
}
Pebble.addEventListener('ready', sendSaved);
Pebble.addEventListener('showConfiguration', function() { Pebble.openURL(clay.generateUrl()); });
Pebble.addEventListener('webviewclosed', function(event) {
  if (!event || !event.response || event.response === 'CANCELLED') return;
  try {
    clay.getSettings(event.response, false);
    sendSaved();
  } catch (error) { console.log('Dot Matrix settings were not saved'); }
});

module.exports = {normalize: normalize, defaults: defaults};
