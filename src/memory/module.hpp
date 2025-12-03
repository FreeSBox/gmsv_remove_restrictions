/**
 * @file module.hpp
 *
 * @copyright GNU General Public License Version 2.
 * This file is part of YimMenu.
 * YimMenu is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version. YimMenu is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details. You should have received a copy of the GNU General Public License
 * along with YimMenu. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include "range.hpp"
#include <string_view>


namespace memory
{
	class module : public range
	{
	public:
		explicit module(const std::string_view name);

		bool loaded() const;

	protected:
		bool try_get_module();


	private:
		const std::string_view m_name;
		bool m_loaded;
	};
}
