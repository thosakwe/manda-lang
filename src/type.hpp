#ifndef W_TYPE_HPP
#define W_TYPE_HPP

namespace w_lang {
  class Type {
    public:
      virtual ~Type() = default;
      const Type* getInnermost() const;
  };
}

#endif
