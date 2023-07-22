#pragma once

#include "const.h"

namespace leap {
	namespace pos {
		/**
		 * \brief The type base of a point. For library users, use Point<...> instead.
		 * \tparam Arithmetic The arithmetic type of the point, int, float, etc.
		 */
		template <typename Arithmetic>
		struct point_type;

		template <typename Arithmetic>
		struct point_type {
			struct type {
				Arithmetic x, y;
			};
		};

		template <>
		struct point_type<int> {
			using type = SDL_Point;
		};

		template <>
		struct point_type<float> {
			using type = SDL_FPoint;
		};

		template <typename Arithmetic>
		using point_type_t = typename point_type<Arithmetic>::type;


		template <typename Arithmetic>
		class Point : public point_type_t<Arithmetic> {
		public:
			Point() noexcept: point_type_t<Arithmetic>{0, 0} { }

			Point(Arithmetic x, Arithmetic y) noexcept: point_type_t<Arithmetic>{x, y} { }

			Point operator+(const Point &p) const noexcept {
				return Point(this->x + p.x, this->y + p.y);
			}

			Point &operator+=(const Point &p) noexcept {
				this->x += p.x;
				this->y += p.y;
				return *this;
			}

			Point operator-(const Point &p) const noexcept {
				return Point(this->x - p.x, this->y - p.y);
			}

			Point &operator-=(const Point &p) {
				this->x -= p.x;
				this->y -= p.y;
				return *this;
			}

			Point operator*(Arithmetic m) const noexcept {
				return Point(this->x * m, this->y * m);
			}

			Point &operator*=(Arithmetic m) noexcept {
				this->x *= m;
				this->y *= m;
				return *this;
			}

			Point operator/(Arithmetic m) const noexcept {
				return Point(this->x / m, this->y / m);
			}

			Point &operator/=(Arithmetic m) noexcept {
				this->x /= m;
				this->y /= m;
				return *this;
			}

			Arithmetic scalar(const Point &p) const noexcept {
				return this->x * p.x + this->y * p.y;
			}

			Arithmetic vector(const Point &p) const noexcept {
				return this->x * p.y - this->y * p.x;
			}

			Arithmetic length() const noexcept {
				return sqrt(this->x * this->x + this->y * this->y);
			}

			operator SDL_Point() const noexcept {
				return SDL_Point{static_cast<int>(this->x), static_cast<int>(this->y)};
			}

			operator SDL_FPoint() const noexcept {
				return SDL_FPoint{static_cast<float>(this->x), static_cast<float>(this->y)};
			}
		};

		using IPoint = Point<int>;
		using FPoint = Point<float>;

		const IPoint origin(0, 0);

		/**
		 * \brief The type base of a rectangle. For library users, use Rect<...> instead.
		 * \tparam Arithmetic The arithmetic type of the rectangle, int, float, etc.
		 */
		template <typename Arithmetic>
		struct rect_type;

		template <typename Arithmetic>
		struct rect_type {
			struct type {
				Arithmetic x, y, w, h;
			};
		};

		template <>
		struct rect_type<int> {
			using type = SDL_Rect;
		};

		template <>
		struct rect_type<float> {
			using type = SDL_FRect;
		};

		template <typename Arithmetic>
		using rect_type_t = typename rect_type<Arithmetic>::type;

		template <typename Arithmetic>
		class Rect : public rect_type_t<Arithmetic> {
			using Point = Point<Arithmetic>;

		public:
			Rect() noexcept : rect_type_t<Arithmetic>{0, 0, 0, 0} { }

			Rect(Arithmetic x, Arithmetic y, Arithmetic w, Arithmetic h) noexcept :
				rect_type_t<Arithmetic>{x, y, w, h} { }

			/*
			 * This is deprecated because it is ambiguous with the constructor Point(Arithmetic, Arithmetic).
			Rect(Arithmetic w, Arithmetic h) noexcept : rect_type_t<Arithmetic>{0, 0, w, h} { }
			*/

			Rect(const Point &from, const Point &to) noexcept :
				rect_type_t<Arithmetic>{from.x, from.y, to.x - from.x, to.y - from.y} { }

			Rect operator+(const Point &p) const noexcept {
				return Rect(this->x + p.x, this->y + p.y, this->w, this->h);
			}

			Rect &operator+=(const Point &p) noexcept {
				this->x += p.x;
				this->y += p.y;
				return *this;
			}

			Rect operator-(const Point &p) const noexcept {
				return Rect(this->x - p.x, this->y - p.y, this->w, this->h);
			}

			Rect operator-=(const Point &p) noexcept {
				this->x -= p.x;
				this->y -= p.y;
				return *this;
			}

			Rect operator*(Arithmetic m) const noexcept {
				return Rect(this->x, this->y, this->w * m, this->h * m);
			}

			Rect &operator*=(Arithmetic m) noexcept {
				this->w *= m;
				this->h *= m;
				return *this;
			}

			Rect operator/(Arithmetic m) const noexcept {
				return Rect(this->x, this->y, this->w / m, this->h / m);
			}

			Rect &operator/=(Arithmetic m) noexcept {
				this->w /= m;
				this->h /= m;
				return *this;
			}

			Point left_up() const noexcept {
				return Point{this->x, this->y};
			}

			Point left_down() const noexcept {
				return Point{this->x, this->y + this->h};
			}

			Point right_up() const noexcept {
				return Point{this->x + this->w, this->y};
			}

			Point right_down() const noexcept {
				return Point{this->x + this->w, this->y + this->h};
			}

			Point center() const noexcept {
				return Point{this->x + this->w / 2, this->y + this->h / 2};
			}

			Point size() const noexcept {
				return Point{this->w, this->h};
			}

			/**
			 * \brief returns where the rectangle should be placed so that it is centered in dst_rect
			 * \param dst_rect the rectangle that the current rectangle should be centered in
			 * \return a point that represents the top left corner of the rectangle that can make the current rectangle centered in dst_rect
			 */
			Point center_where(const Rect &dst_rect) {
				return {
					(dst_rect->w - this->w) / 2 + dst_rect.x,
					(dst_rect->h - this->h) / 2 + dst_rect.y
				};
			}


			/**
			 * \brief returns a rectangle that is centered in dst_rect
			 * \param dst_rect the rectangle that the current rectangle should be centered in
			 * \return a rectangle that has the same size as the current rectangle and is centered in dst_rect
			 */
			Rect centered(const Rect &dst_rect) {
				return {
					(dst_rect.w - this->w) / 2 + dst_rect.x,
					(dst_rect.h - this->h) / 2 + dst_rect.y,
					this->w,
					this->h
				};
			}

			/**
			 * \brief shrinks the rectangle by p in pixels, while the center stays the same
			 * \param p the size that width and height decreases(or increases if p is negative)
			 * \return A new rectangle instance that has the same center point as the original one and the width and height decreased by p on each side.
			 */
			Rect shrink(int p) const noexcept {
				return Rect(this->x + p, this->y + p, this->w - p * 2, this->h - p * 2);
			}

			/**
			 * \brief Expands the rectangle by \c m times.
			 * \param origin The origin point, or the point that does not move relatively during the expansion.
			 * \param m The times to expand.
			 * \return A new rectangle instance that is expanded from the original one by m times while the origin point is fixed.
			 */
			Rect expand(const Point &origin, Arithmetic m) const noexcept {
				return Rect((m * (this->x - origin.x)) + origin.x,
				            (m * (this->y - origin.y)) + origin.y,
				            this->w * m,
				            this->h * m);
			}


			bool contains(const Point &point) const noexcept {
				return this->x <= point.x && point.x <= this->x + this->w && this->y <= point.y &&
					point.y <= this->y + this->h;
			}

			void move_to(const Point &point) {
				this->x = point.x;
				this->y = point.y;
			}

			operator SDL_Rect() const noexcept {
				return SDL_Rect{
					static_cast<int>(this->x), static_cast<int>(this->y), static_cast<int>(this->w),
					static_cast<int>(this->h)
				};
			}

			operator SDL_FRect() const noexcept {
				return SDL_FRect{
					static_cast<float>(this->x), static_cast<float>(this->y), static_cast<float>(this->w),
					static_cast<float>(this->h)
				};
			}
		};

		using IRect = Rect<int>;
		using FRect = Rect<float>;
	}
}
