#pragma once

#include <ostream>
#include <sstream>
#include <tuple>
#include <variant>

namespace orpg
{
   struct house1
   {
      friend auto& operator<<(std::ostream& os, house1 const&) { return os << "house1"; }
   };

   struct house2
   {
      friend auto& operator<<(std::ostream& os, house2 const&) { return os << "house2"; }
   };

   struct house3
   {
      friend auto& operator<<(std::ostream& os, house3 const&) { return os << "house3"; }
   };

   struct world1
   {
      friend auto& operator<<(std::ostream& os, world1 const&) { return os << "world1"; }
   };

   struct test_world
   {
      friend auto& operator<<(std::ostream& os, test_world const&) { return os << "TEST WORLD"; }
   };

   using map_tags_t = std::variant<house1, house2, house3, world1, test_world>;

   auto& operator<<(std::ostream& os, map_tags_t const& obj)
   {
      std::visit([&os](auto e) { os << e; }, obj);
      return os;
   }

   auto to_string(map_tags_t const& obj)
   {
      std::stringstream result{};
      result << obj;
      return result.str();
   }

   template <typename Tag, typename Tuple, auto Index>
   constexpr auto index_of_tag_in_tuple_impl()
   {
      static_assert(Index < std::tuple_size_v<Tuple>, "The element is not in the tuple");

      using tuple_elem_t = std::tuple_element_t<Index, Tuple>;
      using tag_t = typename tuple_elem_t::tag_t;

      if constexpr (std::is_same_v<Tag, tag_t>) {
         return Index;
      } else {
         return index_of_tag_in_tuple_impl<Tag, Tuple, Index + 1>();
      }
   }

   template <typename Tag, typename Tuple>
   struct index_of_tag_in_tuple
   {
      static constexpr auto value = index_of_tag_in_tuple_impl<Tag, Tuple, 0>();
   };

   template <typename Tag, typename Tuple>
   static constexpr auto index_of_tag_in_tuple_v = index_of_tag_in_tuple<Tag, Tuple>::value;

   template <typename Tag, typename T>
   struct tile_map_by_tag;

   template <typename Tag, typename... Ts>
   class tile_map_by_tag<Tag, std::variant<Ts...>>
   {
      using tuple_t = std::tuple<Ts...>;

   public:
      using type_t = std::tuple_element_t<index_of_tag_in_tuple_v<Tag, tuple_t>, tuple_t>;
   };

   template <typename Tag, typename... Ts>
   using tile_map_by_tag_t = typename tile_map_by_tag<Tag, Ts...>::type_t;
} // namespace orpg
