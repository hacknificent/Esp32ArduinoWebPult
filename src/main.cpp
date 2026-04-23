// Esp32 Arduino WebPult for RGB Led lamp/strips
#include <WiFi.h>
#include <IRremote.hpp>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <vector>
#include "config.h" // Contains project settings

// CSS Styles
const String MAIN_CSS = "body {background-color: #eee;min-height: 100vh;display: flex;flex-direction: column;justify-content: flex-end;margin: 0;}.pult-triggers-menu {display: flex;flex-wrap: wrap;gap: 8px;padding: 8px;}.pult-triggers-menu button {flex-basis: calc(50% - 4px);padding: 8px 16px;}.btn-all {font-weight: 700;}.btn-all:hover {opacity: 0.9;}.btn-all[data-btn-action=on] {background-color: #ff4a4a;}.btn-all[data-btn-action=off] {color: #fff;background-color: #303031;}.btn-row {display: flex;justify-content: space-between;gap: 16px;}.pult {background-color: #fff;margin: 16px 0;padding: 16px;border-radius: 15px;box-shadow: 1px 1px 5px #000;}.pult button {display: block;}.pult .btn {position: relative;border-radius: 100%;aspect-ratio: 1;border: none;box-shadow: 0px 0px 3px #000;margin-bottom: 16px;min-width: 32px;}.pult .btn svg {margin: 0 auto;display: inline-block;}.pult .power-btn {background-color: red;position: relative;}.pult .power-btn:before {content: "";position: absolute;left: 15%;right: 15%;top: 15%;bottom: 15%;border-radius: 100%;border: 1.5px solid #000;}.pult .power-btn:after {content: "";position: absolute;left: 50%;top: 5%;transform: translateX(-50%);width: 1.5px;height: 50%;border: 3px solid red;background-color: #000;}.pult .rounded-brtns-wrap {background-color: #eee;border-radius: 100%;}.pult .rounded-brtns-wrap .btn-row {justify-content: center;}.pult-1 {max-width: 124px;}.pult-2 .rgb-btn-wrap {display: flex;width: 100%;justify-content: center;margin-top: 16px;}.pult-3 {display: grid;grid-template-columns: repeat(4, 1fr);grid-template-rows: repeat(5, 64px);gap: 10px;background-color: #252525;}.pult-3 .btn {margin: 0;background-color: #d7d7d7;font-weight: 600;font-size: 12px;}.pult-3 .btn span {position: absolute;top: 50%;left: 56%;font-size: 12px;color: #000;}.pult-3 .btn:nth-child(4), .pult-3 .btn:nth-child(5) {background-color: red;color: white;}.pult-3 .btn:nth-child(6) {background-color: green;color: white;}.pult-3 .btn:nth-child(7) {background-color: blue;color: white;}.pult-3 .btn:nth-child(8) {background-color: #fff;}.pult-3 .btn:nth-child(9) {background-color: orange;}.pult-3 .btn:nth-child(10) {background-color: yellow;}.pult-3 .btn:nth-child(11) {background-color: cyan;}.pult-3 .btn:nth-child(12) {background-color: purple;color: white;}.pult-3 .btn:nth-child(17), .pult-3 .btn:nth-child(18), .pult-3 .btn:nth-child(19), .pult-3 .btn:nth-child(20) {font-size: 24px;}.pult-3 .btn:nth-child(17) {color: red;}.pult-3 .btn:nth-child(18) {color: green;}.pult-3 .btn:nth-child(19) {color: blue;}.pult-3 .btn:nth-child(20) {color: purple;}.pult-4 {display: grid;grid-template-columns: repeat(4, 1fr);grid-template-rows: repeat(5, 60px);gap: 10px;}.pult-4 .btn {margin-bottom: 0;}.pult-wrap {position: fixed;left: 0;right: 0;top: 0;bottom: 0;overflow-y: auto;display: flex;justify-content: center;align-items: center;background-color: hwb(0deg 0% 100%/50%);opacity: 0;pointer-events: none;}.pult-wrap:has(.pult.active) {opacity: 1;pointer-events: all;}.pult-wrap:has(.pult-4) {padding-top: 32px;}.pult-wrap .pult-trigger {display: block;margin: 0 auto;}.pult-trigger {padding: 8px 16px;}";
const String PULTS_LAYOUT = "<div class=\"pult-triggers-menu\"><button data-btn-action=\"disco\" data-action-type=\"customAction\">&#127926; Disco &#128131; &#128378;</button><button data-btn-action=\"regular\" data-action-type=\"customAction\">Обычный</button><button class=\"pult-trigger\" data-pult-number=\"2\">Главная Люстра</button><button class=\"pult-trigger\" data-pult-number=\"1\">Вторая люстра + LED</button><button class=\"pult-trigger\" data-pult-number=\"3\">LED 1</button><button class=\"pult-trigger\" data-pult-number=\"4\">LED 2</button><button class=\"btn-all\" data-btn-action=\"on\" data-action-type=\"customAction\">ON All</button><button class=\"btn-all\" data-btn-action=\"off\" data-action-type=\"customAction\">OFF All</button></div><div class=\"pult-wrap\"><div><div class=\"pult pult-1\" data-pult-number=\"1\"><div class=\"btn-row\"><button data-btn-action=\"0xFD02EF00\" class=\"btn\">ON</button><button data-btn-action=\"0xFC03EF00\" class=\"btn\">OFF</button></div><div class=\"rounded-brtns-wrap\"><div class=\"btn-row\"><button data-btn-action=\"0xF609EF00\" class=\"btn\">G</button></div><div class=\"btn-row\"><button data-btn-action=\"0xF30CEF00\" class=\"btn\">R</button><button data-btn-action=\"0xFF00EF00\" class=\"btn\"><svg xmlns=\"http://www.w3.org/2000/svg\" width=\"32px\" height=\"32px\" viewBox=\"0 0 24 24\" fill=\"none\" style=\"transform:translateX(-8px)\"><path d=\"M13.4349 19.9775L13.4747 19.4791L13.4349 19.9775ZM11.7155 19.4863L12.2035 19.3776L11.7155 19.4863ZM13.9404 6.21305L14.2222 5.80003L13.9404 6.21305ZM20.5 13C20.5 9.41015 17.5899 6.5 14 6.5V5.5C18.1421 5.5 21.5 8.85786 21.5 13H20.5ZM14 19.5C17.5899 19.5 20.5 16.5899 20.5 13H21.5C21.5 17.1421 18.1421 20.5 14 20.5V19.5ZM13.4747 19.4791C13.6479 19.4929 13.8231 19.5 14 19.5V20.5C13.7965 20.5 13.5947 20.4919 13.3951 20.4759L13.4747 19.4791ZM13.2135 17.6509C15.1776 16.5315 16.5 14.4196 16.5 11.9995H17.5C17.5 14.7933 15.9721 17.2297 13.7087 18.5197L13.2135 17.6509ZM16.5 11.9995C16.5 9.76698 15.375 7.79713 13.6586 6.62607L14.2222 5.80003C16.1999 7.14942 17.5 9.42255 17.5 11.9995H16.5ZM13.3951 20.4759C12.8671 20.4338 12.4103 20.3987 12.0806 20.3209C11.7653 20.2464 11.3352 20.0786 11.2275 19.595L12.2035 19.3776C12.1761 19.2543 12.0593 19.2884 12.3104 19.3476C12.5472 19.4036 12.91 19.434 13.4747 19.4791L13.3951 20.4759ZM13.7087 18.5197C13.0872 18.874 12.6637 19.1164 12.3998 19.3123C12.2677 19.4104 12.2131 19.4702 12.1949 19.4966C12.1858 19.5098 12.2216 19.4586 12.2035 19.3776L11.2275 19.595C11.1693 19.3338 11.251 19.1041 11.3715 18.9292C11.4829 18.7675 11.6415 18.6298 11.8037 18.5094C12.1284 18.2683 12.6178 17.9904 13.2135 17.6509L13.7087 18.5197ZM14 6.5C14.1966 6.5 14.3351 6.35807 14.3744 6.21479C14.4123 6.07668 14.3702 5.90097 14.2222 5.80003L13.6586 6.62607C13.4166 6.46096 13.3473 6.17873 13.4101 5.95013C13.4742 5.71637 13.6895 5.5 14 5.5V6.5Z\" fill=\"#222222\" /><path d=\"M7.4 11.2L7.4 11.2C7.50137 11.5041 7.55206 11.6562 7.60276 11.7225C7.80288 11.9843 8.19712 11.9843 8.39724 11.7225C8.44794 11.6562 8.49863 11.5041 8.6 11.2L8.6 11.2C8.68177 10.9547 8.72266 10.832 8.77555 10.721C8.97291 10.3067 9.30672 9.97291 9.72099 9.77555C9.83201 9.72266 9.95468 9.68177 10.2 9.6L10.2 9.6C10.5041 9.49863 10.6562 9.44794 10.7225 9.39724C10.9843 9.19712 10.9843 8.80288 10.7225 8.60276C10.6562 8.55206 10.5041 8.50137 10.2 8.4L10.2 8.4C9.95468 8.31822 9.83201 8.27734 9.72099 8.22445C9.30672 8.02709 8.97291 7.69329 8.77555 7.27901C8.72266 7.16799 8.68177 7.04532 8.6 6.8C8.49863 6.49588 8.44794 6.34382 8.39724 6.2775C8.19712 6.01569 7.80288 6.01569 7.60276 6.2775C7.55206 6.34382 7.50137 6.49588 7.4 6.8C7.31823 7.04532 7.27734 7.16799 7.22445 7.27901C7.02709 7.69329 6.69329 8.02709 6.27901 8.22445C6.16799 8.27734 6.04532 8.31823 5.8 8.4C5.49588 8.50137 5.34382 8.55206 5.2775 8.60276C5.01569 8.80288 5.01569 9.19712 5.2775 9.39724C5.34382 9.44794 5.49588 9.49863 5.8 9.6C6.04532 9.68177 6.16799 9.72266 6.27901 9.77555C6.69329 9.97291 7.02709 10.3067 7.22445 10.721C7.27734 10.832 7.31822 10.9547 7.4 11.2Z\" stroke=\"#222222\" /></svg></button><button data-btn-action=\"0xF10EEF00\" class=\"btn\">B</button></div><div class=\"btn-row\"><button data-btn-action=\"0xEE11EF00\" class=\"btn\" style=\"margin-bottom: 0;\">CW</button></div></div><div class=\"btn-row\"><button data-btn-action=\"0xF40BEF00\" class=\"btn\"><svg width=\"26\" height=\"26\"><use href=\"#icon-sun-small\"></button><button data-btn-action=\"0xF807EF00\" class=\"btn btn-with-icon\"><svg width=\"26\" height=\"26\"><use href=\"#icon-sun\"></use></svg></button></div><div class=\"btn-row\" style=\"justify-content: center\"><button data-btn-action=\"0xEB14EF00\" class=\"btn\">Mode</button></div><div class=\"btn-row\"><button data-btn-action=\"0xE817EF00\" class=\"btn\">Warm</button><button data-btn-action=\"0xE916EF00\" class=\"btn\">Cool</button></div></div><button class=\"pult-trigger\" data-pult-number=\"3\">Next Pult</button></div></div><div class=\"pult-wrap\"><div><div class=\"pult pult-2\" data-pult-number=\"2\"><button class=\"power-btn btn\" data-btn-action=\"0xFF00FE01\"></button><div class=\"btn-row\"><button data-btn-action=\"0xFE01FE01\" class=\"btn\">B - </button><button data-btn-action=\"0xF609FE01\" class=\"btn\">B + </button></div><div class=\"rounded-brtns-wrap\"><div class=\"btn-row\"><button data-btn-action=\"0xFA05FE01\" class=\"btn\">Vol - </button></div><div class=\"btn-row\"><button data-btn-action=\"0xFD02FE01\" class=\"btn\"><svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\"><!--!Font Awesome Free 6.7.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free Copyright 2025 Fonticons, Inc.--><path d=\"M459.5 440.6c9.5 7.9 22.8 9.7 34.1 4.4s18.4-16.6 18.4-29l0-320c0-12.4-7.2-23.7-18.4-29s-24.5-3.6-34.1 4.4L288 214.3l0 41.7 0 41.7L459.5 440.6zM256 352l0-96 0-128 0-32c0-12.4-7.2-23.7-18.4-29s-24.5-3.6-34.1 4.4l-192 160C4.2 237.5 0 246.5 0 256s4.2 18.5 11.5 24.6l192 160c9.5 7.9 22.8 9.7 34.1 4.4s18.4-16.6 18.4-29l0-64z\" /></svg></button><button data-btn-action=\"0xF906FE01\" class=\"btn\"><svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 384 512\"><!--!Font Awesome Free 6.7.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free Copyright 2025 Fonticons, Inc.--><path d=\"M73 39c-14.8-9.1-33.4-9.4-48.5-.9S0 62.6 0 80L0 432c0 17.4 9.4 33.4 24.5 41.9s33.7 8.1 48.5-.9L361 297c14.3-8.7 23-24.2 23-41s-8.7-32.2-23-41L73 39z\" /></svg> </button><button data-btn-action=\"0xF50AFE01\" class=\"btn\"><svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 512 512\"><!--!Font Awesome Free 6.7.2 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free Copyright 2025 Fonticons, Inc.--><path d=\"M52.5 440.6c-9.5 7.9-22.8 9.7-34.1 4.4S0 428.4 0 416L0 96C0 83.6 7.2 72.3 18.4 67s24.5-3.6 34.1 4.4L224 214.3l0 41.7 0 41.7L52.5 440.6zM256 352l0-96 0-128 0-32c0-12.4 7.2-23.7 18.4-29s24.5-3.6 34.1 4.4l192 160c7.3 6.1 11.5 15.1 11.5 24.6s-4.2 18.5-11.5 24.6l-192 160c-9.5 7.9-22.8 9.7-34.1 4.4s-18.4-16.6-18.4-29l0-64z\" /></svg></button></div><div class=\"btn-row\"><button data-btn-action=\"0xFB04FE01\" class=\"btn\" style=\"margin-bottom: 0;\">Vol + </button></div></div><div class=\"btn-row\"><div class=\"rgb-btn-wrap\"><button data-btn-action=\"0xFC03FE01\" class=\"btn rgb-btn\">R</button><button data-btn-action=\"0xF807FE01\" class=\"btn rgb-btn\">G</button><button data-btn-action=\"0xF40BFE01\" class=\"btn rgb-btn\">B</button></div></div><div class=\"btn-row\"><button data-btn-action=\"0xBF40FE01\" class=\"btn\">Warm</button><button data-btn-action=\"0xB748FE01\" class=\"btn\">Cool</button></div><div class=\"btn-row\" style=\"justify-content: center\"><button data-btn-action=\"0xBB44FE01\" class=\"btn\">Mode</button></div></div><button class=\"pult-trigger\" data-pult-number=\"1\">Next Pult</button></div></div><div class=\"pult-wrap\"><div><div class=\"pult pult-3\" data-pult-number=\"3\"><button data-btn-action=\"0xFF00EF00\" class=\"btn\"><svg width=\"26\" height=\"26\"><use href=\"#icon-sun\"></button><button data-btn-action=\"0xFE01EF00\" class=\"btn\"><svg width=\"26\" height=\"26\"><use href=\"#icon-sun-small\"></button><button data-btn-action=\"0xFD02EF00\" class=\"btn\">ON</button><button data-btn-action=\"0xFC03EF00\" class=\"btn\">OFF</button><button data-btn-action=\"0xFB04EF00\" class=\"btn\">RED</button><button data-btn-action=\"0xFA05EF00\" class=\"btn\">GREEN</button><button data-btn-action=\"0xF906EF00\" class=\"btn\">BLUE</button><button data-btn-action=\"0xF807EF00\" class=\"btn\">WHITE</button><button data-btn-action=\"0xF708EF00\" class=\"btn\">ORANGE</button><button data-btn-action=\"0xF609EF00\" class=\"btn\">YELLOW</button><button data-btn-action=\"0xF50AEF00\" class=\"btn\">CYAN</button><button data-btn-action=\"0xF40BEF00\" class=\"btn\">PURPLE</button><button data-btn-action=\"0xF30CEF00\" class=\"btn\">JUMP 3</button><button data-btn-action=\"0xF20DEF00\" class=\"btn\">JUMP 7</button><button data-btn-action=\"0xF10EEF00\" class=\"btn\">FADE 3</button><button data-btn-action=\"0xF00FEF00\" class=\"btn\">FADE 7</button><button data-btn-action=\"0xEF10EF00\" class=\"btn\">&#9834; <span>1</span></button><button data-btn-action=\"0xEE11EF00\" class=\"btn\">&#9834; <span>2</span></button><button data-btn-action=\"0xED12EF00\" class=\"btn\">&#9834; <span>3</span></button><button data-btn-action=\"0xEC13EF00\" class=\"btn\">&#9834; <span>4</span></button></div><button class=\"pult-trigger\" data-pult-number=\"4\">Next Pult</button></div></div><div class=\"pult-wrap\"><div><div class=\"pult pult-4\" data-pult-number=\"4\"><button data-btn-action=\"0xA35CFF00\" class=\"btn\"><svg width=\"26\" height=\"26\"><use href=\"#icon-sun\"></button><button data-btn-action=\"0xA25DFF00\" class=\"btn\"><svg width=\"26\" height=\"26\"><use href=\"#icon-sun-small\"></button><button data-btn-action=\"0xBE41FF00\" class=\"btn\"><svg viewBox=\"0 0 24 24\"><path d=\"M5 5v14l10-7zM17 5h3v14h-3z\" /></svg></button><button data-btn-action=\"0xBF40FF00\" class=\"btn power-btn\"></button><button data-btn-action=\"0xA758FF00\" class=\"btn\">RED</button><button data-btn-action=\"0xA659FF00\" class=\"btn\">GREEN</button><button data-btn-action=\"0xBA45FF00\" class=\"btn\">BLUE</button><button data-btn-action=\"0xBB44FF00\" class=\"btn\">WHITE</button><button data-btn-action=\"0xAB54FF00\" class=\"btn\"></button><button data-btn-action=\"0xAA55FF00\" class=\"btn\"></button><button data-btn-action=\"0xB649FF00\" class=\"btn\"></button><button data-btn-action=\"0xB748FF00\" class=\"btn\"></button><button data-btn-action=\"0xAF50FF00\" class=\"btn\"></button><button data-btn-action=\"0xAE51FF00\" class=\"btn\"></button><button data-btn-action=\"0xB24DFF00\" class=\"btn\"></button><button data-btn-action=\"0xB34CFF00\" class=\"btn\"></button><button data-btn-action=\"0xE31CFF00\" class=\"btn\"></button><button data-btn-action=\"0xE21DFF00\" class=\"btn\"></button><button data-btn-action=\"0xE11EFF00\" class=\"btn\"></button><button data-btn-action=\"0xE01FFF00\" class=\"btn\"></button><button data-btn-action=\"0xE718FF00\" class=\"btn\"></button><button data-btn-action=\"0xE619FF00\" class=\"btn\"></button><button data-btn-action=\"0xE51AFF00\" class=\"btn\"></button><button data-btn-action=\"0xE41BFF00\" class=\"btn\"></button><button data-btn-action=\"0xEB14FF00\" class=\"btn\">JUMP 3</button><button data-btn-action=\"0xEA15FF00\" class=\"btn\">JUMP 7</button><button data-btn-action=\"0xE916FF00\" class=\"btn\">FADE 3</button><button data-btn-action=\"0xE817FF00\" class=\"btn\">FADE 7</button><button data-btn-action=\"0xEF10FF00\" class=\"btn\"></button><button data-btn-action=\"0xEE11FF00\" class=\"btn\"></button><button data-btn-action=\"0xED12FF00\" class=\"btn\"></button><button data-btn-action=\"0xEC13FF00\" class=\"btn\"></button><button data-btn-action=\"0xF30CFF00\" class=\"btn\"></button><button data-btn-action=\"0xF20DFF00\" class=\"btn\"></button><button data-btn-action=\"0xF10EFF00\" class=\"btn\"></button><button data-btn-action=\"0xF00FFF00\" class=\"btn\"></button><button data-btn-action=\"0xF708FF00\" class=\"btn\">&#9834; <span>1</span></button><button data-btn-action=\"0xF609FF00\" class=\"btn\">&#9834; <span>2</span></button><button data-btn-action=\"0xF50AFF00\" class=\"btn\">&#9834; <span>3</span></button><button data-btn-action=\"0xF40BFF00\" class=\"btn\">&#9834; <span>4</span></button></div><button class=\"pult-trigger\" data-pult-number=\"2\">Next Pult</button></div></div><!-- Inline Sprite--><svg xmlns=\"http://www.w3.org/2000/svg\" style=\"display: none;\"><symbol id=\"icon-sun\" viewBox=\"0 0 24 24\"><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M12 7.5C9.51472 7.5 7.5 9.51472 7.5 12C7.5 14.4853 9.51472 16.5 12 16.5C14.4853 16.5 16.5 14.4853 16.5 12C16.5 9.51472 14.4853 7.5 12 7.5ZM6 12C6 8.68629 8.68629 6 12 6C15.3137 6 18 8.68629 18 12C18 15.3137 15.3137 18 12 18C8.68629 18 6 15.3137 6 12Z\" fill=\"currentColor\"></path><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M12.75 3V5.25H11.25V3H12.75Z\" fill=\"currentColor\"></path><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M21 12.75L18.75 12.75L18.75 11.25L21 11.25L21 12.75Z\" fill=\"currentColor\"></path><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M18.8943 6.16637L17.3033 7.75736L16.2426 6.6967L17.8336 5.10571L18.8943 6.16637Z\" fill=\"currentColor\"></path><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M17.8336 18.8944L16.2426 17.3034L17.3033 16.2428L18.8943 17.8337L17.8336 18.8944Z\" fill=\"currentColor\"></path><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M12.75 18.75V21H11.25V18.75H12.75Z\" fill=\"currentColor\"></path><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M5.25 12.75L3 12.75L3 11.25L5.25 11.25L5.25 12.75Z\" fill=\"currentColor\"></path><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M7.75732 17.3033L6.16633 18.8943L5.10567 17.8337L6.69666 16.2427L7.75732 17.3033Z\" fill=\"currentColor\"></path><path fill-rule=\"evenodd\" clip-rule=\"evenodd\" d=\"M6.69666 7.75744L5.10567 6.16645L6.16633 5.10579L7.75732 6.69678L6.69666 7.75744Z\" fill=\"currentColor\"></path></symbol></svg><svg xmlns=\"http://www.w3.org/2000/svg\" style=\"display:none\"><symbol id=\"icon-sun-small\" viewBox=\"0 0 24 24\"><circle cx=\"12\" cy=\"12\" r=\"3.5\" stroke=\"currentColor\" stroke-width=\"1.2\" fill=\"none\" /><path d=\"M12 3V6M12 18v3M3 12h3M18 12h3M5.6 5.6l2.1 2.1M16.3 16.3l2.1 2.1M5.6 18.4l2.1-2.1M16.3 7.7l2.1-2.1\" stroke=\"currentColor\" stroke-width=\"1.2\" stroke-linecap=\"round\" /></symbol></svg>";
const String JS_SCRIPTS = "document.addEventListener(\"DOMContentLoaded\",function(){const t=document.querySelectorAll(\".pult-trigger\"),e=document.querySelectorAll(\".pult-wrap\"),n=document.querySelectorAll(\"button[data-btn-action]\");document.querySelectorAll(\"button[data-btn-action]\");t.forEach(t=>{t.addEventListener(\"click\",function(){const t=this.getAttribute(\"data-pult-number\");if(!t)return;const e=document.querySelector(\".pult.active\");e&&e.classList.remove(\"active\");const n=document.querySelector(`.pult[data-pult-number=\"${t}\"]`);n&&n.classList.add(\"active\")})}),e.forEach(t=>{t.addEventListener(\"click\",function(t){if(t.target===this){this.querySelectorAll(\".pult\").forEach(t=>{t.classList.remove(\"active\")})}})}),n.forEach(t=>{t.addEventListener(\"click\",function(){const t=this.getAttribute(\"data-btn-action\"),e=this.getAttribute(\"data-action-type\")??\"btnAction\";t&&fetch(\"/pultAction\",{method:\"POST\",headers:{\"Content-Type\":\"application/x-www-form-urlencoded\"},body:new URLSearchParams({[e]:t})}).then(t=>{if(!t.ok)throw new Error(\"Network response was not ok\");return t.text()}).then(t=>{console.log(\"Response:\",t)}).catch(t=>{console.error(\"POST request failed:\",t)})})})});";
unsigned long tData = 0xFF00FE01;

LiquidCrystal_I2C lcd(0x27, 16, 2); // See http://playground.arduino.cc/Main/I2cScanner how to test for a I2C device.
WiFiServer server(80);

uint32_t constexpr hash(const char *str, int h = 0)
{
  return !str[h] ? 5381 : (hash(str, h + 1) * 33) ^ str[h];
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Sketch:");
  Serial.println(__FILE__);
  Serial.print("Uploaded: ");
  Serial.println(__DATE__);
  delay(80);

  pinMode(buttonPin, INPUT);                         // set the touch button pin
  IrSender.begin(IRSenderdPin, ENABLE_LED_FEEDBACK); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin

  lcd.init(); // initialize the lcd
  lcd.clear();
  lcd.noBacklight();
  delay(80);

  IrSender.sendNECRaw(0xFD02EF00, 0); // Comand - On additional light
  delay(80);

  // Serial.println();
  // Serial.println();
  // Serial.print("Connecting to ");
  // Serial.println(ssid);


  lcd.setCursor(0, 0);
  lcd.print(ssid);
  delay(80);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("WiFi Alert");
    delay(120);
  }

  // Serial.println("");
  // Serial.println("WiFi connected.");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(WiFi.localIP());
  delay(80);
  lcd.backlight();


  server.begin();
  delay(80);

  IrSender.sendNECRaw(tData, 0); // The function sendNEC(data, nbits) is deprecated and may not work as expected! Use sendNECRaw(data, NumberOfRepeats) or better sendNEC(Address, Command, NumberOfRepeats).
  delay(80);
}

// BtnVars
int currentTouchBtnAction = 0; // Btn Actions: 0 : "whait"; 1 : "action"; 2 : "longAction"; 3: "doubleTap";
int lastTouchActionWas = 0;    // Store information about how long button was not touched

// DisplayVars
bool displayBacklight = true;
int displayBacklightTime = 40; // Sets how long backlight will work after touch btn action
int displayBacklightTimer = displayBacklightTime;

// LampVars
bool nifLampPowerStatement = false;
int nifLampMode = 0; // 0: White; 1: Smooth; (Switches on long-action)

void loop()
{

  WiFiClient client = server.available(); // listen for incoming clients

  // Display backlight swither
  if (displayBacklightTimer > 0)
  {
    if (!displayBacklight)
    {
      lcd.backlight(); // turn on backlight.
      displayBacklight = true;
    }
    displayBacklightTimer--;
    delay(40);
  }
  else
  {
    if (displayBacklight)
    {
      lcd.noBacklight(); // turn off backlight
      displayBacklight = false;
    }
  }

  // Touch Button
  int touchButtonCounter = 0; // increes when touch button triggered
  while (digitalRead(buttonPin))
  { // while touch button is triggered

    if (lastTouchActionWas > 1 && lastTouchActionWas <= 10)
    {                            // if button was touched resentli
      currentTouchBtnAction = 3; // "doubleTap"
      break;
    }
    else
    {
      currentTouchBtnAction = 1; // "action"

      if (++touchButtonCounter >= 10)
      {
        currentTouchBtnAction = 2; // "longAction"
        break;
        //...
      }
    }

    delay(40);
  }

  if (currentTouchBtnAction != 0)
  { // If btn statemant is not "whait"
    if (++lastTouchActionWas >= 10)
    {
      currentTouchBtnAction = 0; // Sets btn statemant as "whait"
      lastTouchActionWas = 0;
    }
  }

  switch (currentTouchBtnAction)
  {
  case 1: // "action"
    displayBacklightTimer = displayBacklightTime;
    break;
  case 2: // "longAction"
    // Change Light Mode
    if (nifLampMode == 0)
    {
      tData = 0xFD02EF00; // On - Pult 1
      nifLampMode = 1;
    }
    else if (nifLampMode == 1)
    {
      tData = 0xFC03EF00; // Off - Pult 1
      nifLampMode = 0;
    }
    IrSender.sendNECRaw(tData, 0);
    delay(40);

    currentTouchBtnAction = 0;
    lastTouchActionWas = 0;
    break;
  case 3: // "doubleTap"
    IrSender.sendNECRaw(0xFF00FE01, 0);
    delay(40);

    currentTouchBtnAction = 0;
    lastTouchActionWas = 0;
    break;
  }

  bool getCommand = false; // Changes when user ask to do pultCommand

  if (client)
  {
    Serial.println("New Client."); // if you get a client,
    String postData = "";
    // print a message out the serial port
    String currentLine = ""; // make a String to hold incoming data from the client

    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("<head>");
            client.print("<meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0\">");
            client.print("<link rel=\"icon\" type=\"image/png\" sizes=\"16x16\" href=\"https://openrgb.org/favicon-16x16.07e0419e.png\">");
            client.print("<style>" + MAIN_CSS + "</style>");
            client.print("</head><body>");

            // client.print("<form method='POST' action='/textForm'>");
            // client.print("<label>Text to LCD: <input name='message' type='text'/></label>");
            // client.print("<input type='submit' value='Send'/>");
            // client.print("</form>");

            client.print(PULTS_LAYOUT);

            // client.print("<form action=\"pultTest\" target=\"_self\" method=\"post\" ><ul class=\"pult\">");
            // client.print("<li class=\"pultButton\"");
            // client.print("style = \"flex-basis: 100%;\">"); // Print button color
            // client.print("<span>Pult Test</span>"); // Print button Label              client.print("<input type=\"submit\" name=\"btn\" value=\"" + String(i) + "c\">");
            // client.print("<input type=\"submit\" name=\"btn\" value=\"Submit\">");
            // client.print("</li>");
            // client.print("</ul></form>");
            client.print("<script type =\"text/javascript\">" + JS_SCRIPTS + "</script>");
            client.print("</body>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }

        Serial.println(currentLine);

        // if (currentLine.startsWith("POST /textForm"))
        // {
        //   while (client.available())
        //   {
        //     postData += (char)client.read();
        //   }
        //   Serial.println(postData);

        //   int textIndex = postData.indexOf("message=");
        //   if (textIndex >= 0)
        //   {
        //     String message = postData.substring(textIndex + 8);
        //     message.replace('+', ' ');
        //     int amp = message.indexOf('&');
        //     if (amp > 0)
        //       message = message.substring(0, amp);

        //     lcd.clear();
        //     lcd.setCursor(0, 0);
        //     lcd.print(message.substring(0, 16));
        //     lcd.setCursor(0, 1);
        //     if (message.length() > 16)
        //     {
        //       lcd.print(message.substring(16, 32));
        //     }
        //     displayBacklightTimer = displayBacklightTime;
        //     client.stop();
        //     break;
        //   }
        // }
        // else
        if (currentLine.startsWith("POST /pultAction"))
        {
          while (client.available())
          {
            postData += (char)client.read();
          }
          Serial.println(postData);

          int btnActionIndex = postData.indexOf("btnAction=");
          int customActionIndex = postData.indexOf("customAction=");
          bool isSuccess = false;

          if (btnActionIndex >= 0)
          {
            String tDataText = postData.substring(btnActionIndex + 10);
            int amp = tDataText.indexOf('&');
            if (amp > 0)
              tDataText = tDataText.substring(0, amp);

            tData = strtoul(tDataText.c_str(), NULL, 16);

            IrSender.sendNECRaw(tData, 0);
            delay(80);

            isSuccess = true;
            break;
          }

          if (customActionIndex >= 0)
          {
            String actionVal = postData.substring(customActionIndex + 13);
            std::vector<unsigned long> pultActionsToDo = {};
            int amp = actionVal.indexOf('&');
            if (amp > 0)
              actionVal = actionVal.substring(0, amp);

            switch (hash(actionVal.c_str()))
            {
            case hash("on"):
              pultActionsToDo = {
                0xBF40FE01, // On Main with Warm button
                0xFD02EF00, // On LEDs & Lamp 2
              };
              isSuccess = true;
              break;
            case hash("off"):
              pultActionsToDo = {
                0xBF40FE01, // Make Main light warmer to prevent unexpected turn ON
                0xFF00FE01, // Off Main
                0xFC03EF00, // Off LEDs & Lamp 2
              };
              isSuccess = true;
              break;
            case hash("disco"):
              pultActionsToDo = {
                0xF708FF00, // Pult 4 - Music 1
                0xEE11EF00, // Pult 3 - Music 2
                0xFC03FE01, // Oukt 1 - Red btn
                0xFC03FE01,
                0xFC03FE01,
                0xFC03FE01,
                0xFC03FE01,
                0xFC03FE01,
                0xFC03FE01,
                0xFC03FE01,
                0xFC03FE01,
                0xFC03FE01,

              };
              isSuccess = true;
              break;
            case hash("regular"):
              pultActionsToDo = {
                0xEE11EF00, // Lamp 2 "CW" button
                0xF40BEF00, // Lamp 2 - Sun Down
                0xF807EF00, // Lamp 2 - Sun Up
                0xBF40FE01, // Main lamp "Warm" btn
              };
              isSuccess = true;
              break;
              
            }
            
            for (auto code : pultActionsToDo) {
              IrSender.sendNECRaw(code, 0);
              delay(120);
            }
          }

          if (isSuccess)
          {
            displayBacklightTimer = displayBacklightTime / 2;
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/plain");
            client.println("Connection: close");
            client.println();
            client.println("Done");
            delay(1);
          }

          client.stop();
          break;
        }
      }
    }
    // close the connection:
    client.stop();
  }
}
