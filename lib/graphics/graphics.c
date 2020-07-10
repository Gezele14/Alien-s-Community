#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>


/*
 * Loads a BMP image into a texture on the rendering device
 * @param file The BMP image file to load
 * @param ren The renderer to load the texture onto
 * @return the loaded texture, or NULL if something went wrong.
 */
SDL_Texture* loadTexture(const char *file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file);
	//If the loading went ok, convert to texture and return the texture
	if (texture == NULL){
		printf("LoadTexture error: %s\n", SDL_GetError());
	}
	return texture;
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
 * width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	//Query the texture to get its width and height to use
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 */
void renderTextureFull(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTextureClip(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip){
	SDL_RenderCopy(ren, tex, clip, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height and taking a clip of the texture if desired
* If a clip is passed, the clip's width and height will be used instead of
*	the texture's
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTextureSheet(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w,SDL_Rect *clip)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != NULL){
		dst.w = w;
		dst.h = w;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTextureClip(tex, ren, dst, clip);
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(char *message, char * fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile, fontSize);
	if (font == NULL){
		printf("TTF_OpenFont error: %s\n", SDL_GetError());
		return NULL;
	}	
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message, color);
	if (surf == NULL){
		TTF_CloseFont(font);
		printf("TTF_RenderText error: %s\n", SDL_GetError());
		return NULL;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == NULL){
		printf("CreateTexture error: %s\n", SDL_GetError());
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

/**
* Calc the rectangle of any Texture for collisions
* @param Tex The texture to calculate de rect
* @param x upper left corner x coordenate of the texture
* @param y upper left corner y coordenate of the texture
* @return A bounding box of the texture.
*/
SDL_Rect getTextureRect(SDL_Texture *Tex, int x, int y){
	SDL_Rect temp;
	int w,h;
	SDL_QueryTexture(Tex, NULL, NULL, &w, &h);

	temp.x = x;
	temp.y = y;
	temp.w = w;
	temp.h = h;
	return temp;
}





